#!/usr/bin/env python3

# Vulkan CTS Wrapper
# Mike Schuchardt <mikes@lunarg.com>
#
# usage:
#   cts_wrapper.py caselist
#
# Run CTS against caselist. If result database already exists then resume
# previous run and ignore caselist.

from collections import OrderedDict
import itertools
import os
import re
import sqlite3
import subprocess
import sys
import time
import traceback

CTS_EXECUTABLE = './deqp-vk'
LOG_FILENAME = 'cts_log.txt'
DB_FILENAME = 'cts_database.sqlite'

# use sqlite for bookkeeping (mostly for the fast atomic updates)
db_con = sqlite3.connect(DB_FILENAME)

# look for existing results in database
if db_con.execute('SELECT name FROM sqlite_master WHERE type="table" AND name="results"').fetchall():
    print("Resuming previous run from {}".format(DB_FILENAME))
    time.sleep(2)
else:
    # new run, initialize the database from the case list
    print('Initializing result database from ' + sys.argv[1])
    db_con.execute('CREATE TABLE results (name STRING, status STRING, output STRING)')
    with open(sys.argv[1]) as caselist_file:
        db_con.executemany('INSERT INTO results VALUES (?, ?, ?)',
                           ((line.strip(), 'NotExecuted', '') for line in caselist_file))
        db_con.commit()

    # reset log file
    if os.path.exists(LOG_FILENAME):
        os.remove(LOG_FILENAME)

def update_db(rowid, status, output):
    db_con.execute('UPDATE results SET status=?, output=? WHERE rowid=?', (status, output, rowid))
    db_con.commit()

# append log_file, line-buffered
with open(LOG_FILENAME, 'a', 1) as log_file:
    try:
        # get list of tests to run
        queue = OrderedDict(db_con.execute('SELECT name,rowid FROM results WHERE status="NotExecuted" ORDER BY rowid'))

        case_count = 0
        case_total = len(queue)
        case_total_width = len(str(case_total))
        case_name = ''
        case_output = ''
        while queue:
            # launch CTS, taking case list over stdin
            cts_proc = subprocess.Popen([CTS_EXECUTABLE, '--deqp-stdin-caselist'],
                                        stdin=subprocess.PIPE,
                                        stdout=subprocess.PIPE,
                                        stderr=subprocess.STDOUT,
                                        bufsize=0) # unbuffered io

            # send test cases in batches of 1000
            cts_proc.stdin.write(''.join((name + '\n' for name in itertools.islice(queue, 1000))).encode('UTF-8'))
            cts_proc.stdin.close()

            while True:
                # read test output
                line = cts_proc.stdout.readline().decode('UTF-8')
                if line:
                    case_output += line
                    log_file.write(line)
                else:
                    break

                # look for test case
                if not case_name:
                    case_match = re.match("Test case '(.*?)'", line)
                    if case_match:
                        case_name = case_match.group(1)
                        # status line
                        case_count += 1
                        print(f'({case_count:>{case_total_width}d}/{case_total}) {case_name}... ',
                              end='', flush=True)
                        # reset test case output
                        case_output = line
                        # TODO: Could spawn watchdog timer here to catch hangs

                # look for test result
                else:
                    status_match = re.match('  (Pass|'
                                               'Fail|'
                                               'QualityWarning|'
                                               'CompatibilityWarning|'
                                               'Pending|'
                                               'NotSupported|'
                                               'ResourceError|'
                                               'InternalError|'
                                               'Crash|'
                                               'Timeout) \\(.*\\)', line)
                    if status_match:
                        status = status_match.group(1)
                        print(status)
                        update_db(queue[case_name], status, case_output)
                        del queue[case_name]
                        case_name = ''
            
            # end of stdout, get return code
            return_code = cts_proc.wait()
            
            # if input ended without a result, we crashed
            if case_name:
                status = 'Crash'
                print(status)
                update_db(queue[case_name], status, case_output)
                del queue[case_name]
                case_name = ''
                # fake a status in the log since the process died
                log_file.write('  {} ({})\n'.format(status, return_code))

            # TODO: could grab TestResults.qpa here and concatenate across
            # multiple runs.  Would likely need to clean up XML fragments left
            # over from crashes.

    except Exception as e:
        status = 'Exception'
        # fake a status in the log since the process was interrupted by us
        log_file.write('  {} ({})\n'.format(status, e.__class__.__name__))
        traceback.print_exc()

    finally:
        # summarize results
        total = db_con.execute('SELECT COUNT(*) FROM results').fetchone()[0]
        total_width = len(str(total))
        print('\nTest run totals:')
        for status, count in db_con.execute('SELECT status, COUNT(status) FROM results GROUP BY status ORDER BY COUNT(status) DESC').fetchall():
            percent = count / total * 100
            print(f'  {status:>14s}: {count:>{total_width}}/{total} ({percent:>5.1f}%)')

        # all done
        db_con.close()

