# Xlate

Xlate is side-by-side translation prettifier.
For detailed syntax, see [example](example.xl).

## Supported OS

Xlate can be compiled on GNU/Linux system and Windows 10. Although on Windows,
compiler generates many a warnings, but it should work preperly.

## Emacs major mode

tools/xlate-mode/xlate-mode.el provides simple syntax highlighting. Add the directory to
your `load-path` and add following code to your `init.el`

```
(require 'xlate-mode)

(add-to-list 'auto-mode-alist '("\\.xl\\'" . xlate-mode))

```
