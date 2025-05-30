# 🔥 Doom Fire Animation with PPM Streaming

A C program that recreates the iconic Doom Fire effect - not in a window, but streamed as raw PPM images directly into a video player like `mpv`.

![YouTube Video](https://img.shields.io/badge/Watch%20on-YouTube-red?logo=youtube)

> 🔗 [Watch the full walkthrough on YouTube](https://www.youtube.com/watch?v=Iy099rEgFjE)

---

## ✨ What It Does

This project simulates the Doom Fire animation by writing uncompressed `.ppm` image frames directly to `stdout`, which are then streamed into `mpv` for display. No graphics libraries or GUI required - just C, pipes, and pixels.

---

## 🔍 Highlights

- Implements the classic Doom Fire pixel decay algorithm
- Generates binary PPM (P6) image frames
- Streams directly to `mpv` via `stdout`
- Simple, fast, and dependency-free

---

## 🧰 Technologies Used

- C Standard Library (`stdio.h`, `stdlib.h`) using C11 standard
- Raw PPM image format (binary, P6)
- `mpv` for real-time frame rendering

---

## 🚀 Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/letsreinventthewheel/doomfire.git
cd doomfire
```

### 2. Build the Program

Use `gcc` (or any C compiler) to compile the project:

```bash
gcc -std=c11 -O3 -o doomfire doomfire.c
```

### 3. Run with `mpv`

```bash
./doomfire | mpv -
```

This command streams the fire animation directly into `mpv`.

---

## 🖼️ What Is a PPM?

A PPM (Portable Pixmap) is a simple image format that stores raw RGB pixel data. This program uses the binary P6 format, writing headers like:

```
P6
WIDTH HEIGHT
255
<binary RGB data>
```

Each frame is a new image sent to `stdout`.

---

## 📺 Full Video Tutorial

Curious how it works under the hood? Watch the [YouTube tutorial](https://www.youtube.com/watch?v=Iy099rEgFjE) where we build this from scratch and explain every step.

---

## 🛠️ Extend It

Here are some ideas to take this project further:

- Add color palette customization
- Output to image files instead of streaming
- Encode to a video using `ffmpeg`
- Make the fire interactive with keyboard input

---

## 💡 Who Is This For?

This project is great for C learners, systems programmers, and graphics enthusiasts who want to explore visual output without GUI frameworks. It also demonstrates how raw image data can be piped and visualized in UNIX-like environments.

---

## 🌐 External Resources

- 🔥 [Doom Fire Algorithm Explained](https://fabiensanglard.net/doom_fire_psx/)
- 📄 [PPM (Portable Pixmap) Format Specification](https://netpbm.sourceforge.net/doc/ppm.html)
- 📺 [mpv Manual](https://mpv.io/manual/master/)
- 🧪 [Working with pipes on the Linux command line](https://www.redhat.com/en/blog/pipes-command-line-linux)

These links provide additional technical background and context for anyone looking to dive deeper into how this project works under the hood.

---

## 🙌 Acknowledgments

Inspired by the `How Doom Fire Was Done` [article](https://fabiensanglard.net/doom_fire_psx/) by Fabien Sanglard
