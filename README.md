# Multithreaded Operating System Kernel

A preemptive, multithreaded kernel designed for the Intel 8086 microprocessor, featuring time-sharing, context switching, semaphores, and event handling. Developed as part of a university project to explore low-level system programming and kernel development.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [Build & Run](#build--run)
- [Directory Structure](#directory-structure)
- [Technical Highlights](#technical-highlights)
- [License](#license)

## Overview

This project implements a rudimentary operating system kernel tailored for the Intel 8086 architecture. It serves as an educational tool to understand the fundamentals of operating system design, including task scheduling, synchronization, and interrupt handling.

## Features

- Preemptive Multitasking: Enables multiple threads to run concurrently with time-sharing.
- Context Switching: Efficiently switches between threads, preserving CPU state.
- Semaphores: Implements synchronization primitives to manage resource access.
- Event Handling: Supports event-driven programming through interrupt handling.
- 8086 Compatibility: Designed specifically for the Intel 8086 microprocessor.

## Architecture

The kernel is structured to separate concerns effectively:

- Scheduler: Manages thread execution order and time slices.
- Thread Management: Handles creation, execution, and termination of threads.
- Synchronization: Provides mechanisms like semaphores for safe resource sharing.
- Interrupt Handling: Manages hardware and software interrupts to respond to events.

## Build & Run

### Prerequisites

- Compiler: A C/C++ compiler compatible with 8086 architecture, such as Borland C++.
- Emulator: An x86 emulator like DOSBox or QEMU.

### Steps

1. Clone the Repository:

   ```bash
   git clone https://github.com/miloshimself/operating-system.git
   cd operating-system
   ```

2. Compile the Source Code:

   Since there's no Makefile, compile the source files manually. Navigate to the `src/` directory and compile each `.cpp` file using your chosen compiler. For example:

   ```bash
   bcc -c src/thread.cpp
   bcc -c src/scheduler.cpp
   bcc -c src/semaphore.cpp
   bcc -c src/event.cpp
   ```

   Then, link the object files to create the executable:

   ```bash
   bcc -o kernel.exe thread.obj scheduler.obj semaphore.obj event.obj
   ```

   Note: Replace `bcc` with the appropriate command for your compiler if different.

3. Run in Emulator:

   Launch the compiled kernel in your chosen emulator:

   ```bash
   dosbox kernel.exe
   ```

   Ensure that `kernel.exe` is located in a directory accessible by the emulator.

## Directory Structure

```
operating-system/
├── BCC/            # Compiler configuration and binaries
├── src/            # Source code files
├── h/              # Header files
├── .settings/      # IDE settings
├── .cproject       # Eclipse project file
├── .project        # Eclipse project file
└── README.md       # Project documentation
```

## Technical Highlights

- Thread Control Blocks (TCBs): Each thread is represented by a TCB containing its state, stack pointer, and other metadata.
- Scheduler Algorithm: Implements a round-robin scheduling algorithm with fixed time slices.
- Semaphore Implementation: Uses counting semaphores to manage access to shared resources.
- Interrupt Service Routines (ISRs): Custom ISRs handle timer interrupts to facilitate preemptive multitasking.