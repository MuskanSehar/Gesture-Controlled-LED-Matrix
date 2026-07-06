# SpotifyClone

A C++ desktop music player built with Raylib. This was a learning project — the main goal was to strengthen object-oriented design in C++ (separating responsibilities cleanly across classes), while using the UI as a sandbox to explore what's possible with a low-level graphics library like Raylib (no UI framework, no image assets — every icon is hand-drawn with primitive shapes).

This isn't a finished, polished app. It's an exploration project, built and iterated on with a friend.

---

## What it does

- Play / pause / skip / previous
- Auto-scans a `music/` folder for `.mp3`, `.wav`, `.ogg` files
- Collapsed mini-player + an expandable full view with a sidebar song list
- Shuffle and repeat
- Volume control (draggable bar + keyboard)
- Progress bar with elapsed/remaining time

---

## Built with

- C++17
- [Raylib](https://www.raylib.com/) 5.5 — windowing, rendering, and audio (pulled in automatically via CMake's `FetchContent`)
- CMake

---

## Project structure

```
SpotifyClone/
├── include/
│   ├── Song.h           # A single track (title, artist, file path)
│   ├── Playlist.h        # A named collection of songs
│   ├── MusicLibrary.h     # Scans a folder and builds the song list
│   ├── Player.h           # Audio playback engine
│   └── UI.h               # UI class declaration
├── src/
│   ├── main.cpp
│   ├── Player.cpp         # Load/play/pause/next/prev/volume logic
│   └── UI.cpp              # All rendering + input handling
├── music/                  # Drop .mp3/.wav/.ogg files here
└── CMakeLists.txt
```

The split between `Song`, `Playlist`, `MusicLibrary`, `Player`, and `UI` was the main point of the exercise — each class has one job, and `Player`/`UI` don't know anything about *how* songs are stored on disk, just that they get handed a `Song` or a `vector<Song>`.

---

## Getting started

**Prerequisites:** CMake 3.16+, a C++17 compiler, Git

```bash
git clone https://github.com/MuskanSehar/SpotifyClone.git
cd SpotifyClone

cmake -B build
cmake --build build

./build/SpotifyClone
```

Raylib downloads and builds automatically on first run via CMake — no manual install needed. Add your own audio files to `music/` before running.

---

## Controls

| Action | Control |
|---|---|
| Play / Pause | `Space` or click the play button |
| Next / Previous track | `→` / `←` or click the buttons |
| Volume up / down | `↑` / `↓` or drag the volume bar |
| Expand / collapse player | Click the mini-player / `Esc` or click ✕ |
| Play a specific song | Click it in the sidebar (expanded view) |
| Toggle shuffle / repeat | Click their icons (expanded view) |

---

## What this was actually for

- Practicing class design and separation of concerns in C++
- Getting hands-on with Raylib — audio streaming, immediate-mode rendering, drawing UI from raw shapes instead of a framework
- Working through a shared codebase with a friend over Git (branches, merges, syncing changes)

It's rough in places and there's plenty I'd improve, but it did what I wanted it to: it forced me to actually think about how to structure a non-trivial C++ project instead of writing everything in one file.

---

## Credits

Built by [Muskan Sehar](https://github.com/MuskanSehar) and [Hassan Mehdi](https://github.com/Hassan-etc).
