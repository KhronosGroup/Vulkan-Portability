# Makefile for Vulkan-Portability sample

EXE_NAME = sample

VULKAN_INCLUDE_DIR = /usr/include
VKPORTABILITY_INCLUDE_DIR = ../include

all: $(EXE_NAME)

$(EXE_NAME): $(EXE_NAME).cpp
	g++ -Wall -std=c++11 -DMOCKDRIVER -I$(VULKAN_INCLUDE_DIR) -I$(VKPORTABILITY_INCLUDE_DIR) -o $@ $<

clean:
	-rm $(EXE_NAME)
