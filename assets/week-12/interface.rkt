#lang racket

(require libserialport)
(require 2htdp/image)
(require 2htdp/universe)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;        DEFINITIONS
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; A World is a (make-world Number [Listof Number])
(define-struct world (curr-val saved-vals))
(define world0 (make-world 0 empty))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;        CONSTANTS
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Reading data
(define BUFFER-SIZE 20000)
(define MAX-VAL 1024)
(define FILTER-TIME-CONST 0.5)

;; Drawing constants
(define W-WIDTH 600)
(define W-HEIGHT 600)
(define BAR-WIDTH (* 0.9 W-WIDTH))
(define BAR-HEIGHT (* 0.2 W-HEIGHT))
(define BAR-OFFSET (/ (- W-WIDTH BAR-WIDTH) 2))

(define BUTTON-HEIGHT 50)
(define BUTTON-WIDTH 100)
(define BUTTON-POS-X (- W-WIDTH BAR-OFFSET (/ BUTTON-WIDTH 2)))
(define BUTTON-POS-Y (+ (* 2 BAR-OFFSET) BAR-HEIGHT (/ BUTTON-HEIGHT 2)))
(define SAVE-TITLE-Y (+ BAR-HEIGHT (* BAR-OFFSET 3) BUTTON-HEIGHT))

(define BG
  (place-image (text "Save" 24 'white)
               BUTTON-POS-X BUTTON-POS-Y
               (place-image (text "SAVED VALUES" 32 'black)
                            (/ W-WIDTH 2)
                            SAVE-TITLE-Y
                            (place-image (rectangle BUTTON-WIDTH BUTTON-HEIGHT
                                                    'solid 'blue)
                                         BUTTON-POS-X BUTTON-POS-Y
                                         (empty-scene W-WIDTH W-HEIGHT)))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;        SERIAL PORT
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; drain-port: InputPort -> InputPort
;; given a port, allocate a buffer of size 'BUFFER SIZE' and
;; repeatedly read available bytes or specials until 0
;; bytes are available.
(define (drain-port port)
  (define buf (make-bytes BUFFER-SIZE))
  (let loop ()
    (define try-read (read-bytes-avail!* buf port))
    (cond [(or (eof-object? try-read)
               (and (number? try-read) (= try-read 0)))
           port]
          [else
           (loop)])))

;; byte->integer : Byte -> Integer
;; Convert a byte to an integer
(define (byte->integer b)
  (first (bytes->list b)))

;; read-framed-byte: InputPort Byte Byte Byte Byte -> Byte
;; Read bytes until the correct framing is found
(define (read-framed-byte port b1 b2 b3 b4)
  (cond [(and (bytes=? #"\1" b1) (bytes=? #"\2" b2)
              (bytes=? #"\3" b3) (bytes=? #"\4" b4))
         (read-bytes 1 port)]
        [else (read-framed-byte port b2 b3 b4 (read-bytes 1 port))]))

;; read-value: None -> Integer
;; Read a value from the port and convert it to 10-bit value
(define (read-value)
  (+ (byte->integer (read-framed-byte (drain-port in) #"\0" #"\0" #"\0" #"\0"))
     (* 256 (byte->integer (read-bytes 1 in)))))

;; filter-val: Number Number -> Number
;; Filter the old value with the new one (alpha filter)
(define (filter-val old new)
  (+ (* (- 1 FILTER-TIME-CONST) old)
     (* FILTER-TIME-CONST new)))

;; val-to-width: Number -> Number
;; Convert the recieved value into a bar width to display in the window
(define (val-to-width val)
     (* BAR-WIDTH (/ val MAX-VAL)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;        DRAW
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; draw-bar: Number Image -> Image
;; Draw a bar showing the received value on the canvas
(define (draw-bar val canvas)
  (place-image (rectangle (val-to-width val) BAR-HEIGHT 'solid 'red)
               (+ BAR-OFFSET (/ (val-to-width val) 2))
               (+ BAR-OFFSET (/ BAR-HEIGHT 2))
               (place-image (rectangle BAR-WIDTH BAR-HEIGHT 'solid 'blue)
                            (+ BAR-OFFSET (/ BAR-WIDTH 2))
                            (+ BAR-OFFSET (/ BAR-HEIGHT 2))
                            canvas)))

;; val-text: Number -> Image
;; Create image of the value to draw
(define (val-text val)
  (text (real->decimal-string val) 24 'black))

;; draw-val: Number Image -> Image
;; Draw text of the received value on the canvas
(define (draw-val val canvas)
  (place-image (val-text val)
               (+ BAR-OFFSET (/ (image-width (val-text val)) 2))
               (+ BAR-HEIGHT (* 2 BAR-OFFSET)
                  (/ (image-height (val-text val)) 2))
               canvas))

;; draw-saved-vals: [Listof Num] Num Image -> Image
;; Draw all the values in the list onto the canvas,
;; incrementing over y-positions
(define (draw-saved-vals nums y canvas)
  (cond [(empty? nums)
         canvas]
        [else
         (draw-saved-vals (rest nums)
                          (+ y (* 1.1 (image-height (val-text (first nums)))))
                          (place-image (val-text (first nums))
                                       (/ W-WIDTH 2) y
                                       canvas))]))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;        UNIVERSE FUNCTIONS
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; draw-world: World -> Image
;; Draw everything in the world (bar, current value, saved values)
(define (draw-world w)
  (draw-bar (world-curr-val w)
            (draw-saved-vals (reverse (world-saved-vals w))
                             (+ SAVE-TITLE-Y (* 2 BAR-OFFSET))
                             (draw-val (world-curr-val w) BG))))

;; world-tick: World -> World
;; Increment one tick in the world
(define (world-tick w)
  (make-world (filter-val (world-curr-val w) (read-value))
              (world-saved-vals w)))

;; mouse-handler: World MouseEvent -> World
;; Save current value when the button is pressed
(define (mouse-handler w x y me)
  (cond [(and (mouse=? me "button-down")
              (< (- BUTTON-POS-X (/ BUTTON-WIDTH 2)) x
                 (+ BUTTON-POS-X (/ BUTTON-WIDTH 2)))
              (< (- BUTTON-POS-Y (/ BUTTON-HEIGHT 2)) y
                 (+ BUTTON-POS-Y (/ BUTTON-HEIGHT 2))))
         (make-world (world-curr-val w)
                     (cons (world-curr-val w)
                           (world-saved-vals w)))]
        [else w]))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;        RUN
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; List serial ports (sanity check)
(for ((serial-port (in-serial-ports)))
    (printf "found ~a\n" serial-port))

; Connect to serial port
(define-values (in out)
    (open-serial-port "/dev/ttyUSB0" #:baudrate 9600))

; Run the universe
(big-bang world0
          (to-draw draw-world)
          (on-tick world-tick .15)
          (on-mouse mouse-handler))
