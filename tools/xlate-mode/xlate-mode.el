;;; xlate-mode.el --- Major mode for editing xlate (*.xl) file    -*- lexical-binding: t -*-
;;
;; Copyright (C) 2019 Koki Fukuda

;; This file is part of xlate.

;; xlate is free software: you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation, either version 3 of the License, or
;; (at your option) any later version.

;; xlate is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GNU Emacs.  If not, see <https://www.gnu.org/licenses/>.

;;; Commentary:

;;; Code:

(setq xlate-mode-highlights
      '(("^#.*$" . font-lock-comment-face)
        ("\\$[^ ]+\\$" . font-lock-variable-name-face)
        ("^!.*$" . font-lock-string-face)))

(define-derived-mode xlate-mode nil "Xlate"
  "Xlate mode"
  (interactive)
  (kill-all-local-variables)
  (setq font-lock-defaults '(xlate-mode-highlights))
  (run-hooks 'xlate-mode-hook))

(provide 'xlate-mode)
