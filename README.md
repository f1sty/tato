# tato

Minimalistic and flexible [Pomodoro](https://en.wikipedia.org/wiki/Pomodoro_Technique) timer TUI
written in C. It's configuration system using `config.h` file is inspired by
[suckless](https://suckless.org/) projects.

## Config

Look into `config.h` file for configuration.

## Compile and install

Just run:

```sh
$ make install
```

in your terminal, and you done. By default `tato` installed in `$HOME/.local/bin` directory.
So be sure to include it into your `$PATH`.

## TODO:

- [x] create `Makefile`
- [x] add `ncurses` library
- [x] clear the screen
- [x] print current status at the centre of the terminal
- [x] postpone on `p` press
- [x] restart session on `r` press
- [x] quit on `q` press
- [x] skip on `s` press
- [x] play alarm sound on state change
- [ ] debug time overflows
- [ ] wrap state into `struct`
- [ ] add optional notification
