
(define letters '("a" "b" "c" "d" "e"
                  "f" "g" "h" "i" "j"
                  "k" "l" "m" "n" "o"
                  "p" "q" "r" "s" "t"
                  "u" "v" "w" "x" "y"
                  "z" "SPACE"))

(define frequency
  '(0.0651738 0.0124248 0.0217339 0.0349835 0.1041442
    0.0197881 0.015861 0.0492888 0.0558094 0.0009033
    0.0050529 0.033149 0.0202124 0.0564513 0.0596302
    0.0137645 0.0008606 0.0497563 0.051576 0.0729357
    0.0225134 0.0082903 0.0171272 0.0013692 0.0145984
    0.0007836 0.1918182))

;Assignment 1:
;procedure that takes the above lists and produces a list of trees: node is weight, left is char, right is null
(define (produce-pairs freq chars) 
  (if (null? freq)
      '()
      (cons (list (car freq) (car chars) '()) (produce-pairs (cdr freq) (cdr chars)))
      )
  )
;test
;(produce-pairs frequency letters)
;((0.0651738 #\a ())
;(0.0124248 #\b ())
;(0.0217339 #\c ())
;(0.0349835 #\d ())
;(0.1041442 #\e ())
;..and so on


;returns frequency from a pair
(define (value p) (car p))

;procedure that returns partially ordered list of pairs based on frequency 
(define (helper-sort L)
  (cond ((null? L) '())
        ((= (length L) 1) L)
        ((<= (value (car L)) (value (list-ref L 1))) (append (list (car L)) (helper-sort (cdr L))))
        (else (helper-sort (append (cdr L) (cons (car L) '()))))
        )
  )

;runs helper-sort on the list (as many times as there are elements in the list) * 5 -not the most efficient but it seems to get the job done
(define (sort L index) ;index starts as 0
  (if (= index (* 5 (length L)))
      L
      (sort (helper-sort L) (+ index 1))
      )
  )
;test
;(sort (produce-pairs frequency letters) 0)
;((0.0007836 #\z ())
; (0.0008606 #\q ())
; (0.0009033 #\j ())
; (0.0013692 #\x ())
; ...and so on

;takes the list and returns it with first two elements removed
(define (remove-first-two L i) ;i starts as 0
  (if (= i 2)
      L
      (remove-first-two (cdr L) (+ i 1))
      )
  )
;test
;(remove-first-two '(1 2 3 4 5) 0)
;(3 4 5)

;creates combined tree with the first two trees, the node being the combined weight, and the left and right being T1 and T2
(define (sub-tree T1 T2)
  (list (+ (car T1) (car T2)) T1 T2)
  )
;test
;(sub-tree '(3 "#\v" '()) '(4 "#\f" '()))
;(7 (3 "#\v" '()) (4 "#\f" '()))

;"main" procedure for creating huff tree, goes through the ordered list of pairs. With each recursion,
;takes two of the least summed values, produces huffman tree,
;and replaces those used values with the new tree in the list, then recurses
;Once there is one element remaining in the list, just returned the combined hoff tree 
(define (hoff-tree L)
  ;(display L)
  (if (= (length L) 1)
      (car L)
      ;pull the least two from list, combine them into tree
      ;recurse with the first two elements of L replaced by new-hoff-tree (and re-sorted)
      (hoff-tree (sort (cons (sub-tree (list-ref L 0) (list-ref L 1)) (remove-first-two L 0)) 0))
      )
  )
;test
;(hoff-tree (sort (produce-pairs frequency letters) 0))
;it does produce a tree, its just to big to paste here and comment all out

;Assignment 2

;Returns the value of the current root
(define (tree-value T) (car T))

;Returns left subtree
(define (tree-left T) (cadr T))

;Returns right subtree
(define (tree-right T) (caddr T))

;to check is a tree is a letter, the right element will be null
(define (letter? T) (null? (tree-right T)))

;determines if the given tree is a hoffman tree vs just a letter
(define (tree? T)
  (if (eq? #t (letter? T))
      #f
      #t)
  )

;takes in a character as an input, returns its encoding
(define (huff-encode-single c T)
  (cond ((and (letter? T) (equal? c (tree-left T))) c) ;if T is a letter and the letter == c
        ((and (letter? T) (not (equal? c (tree-left T)))) "") ;if T is a letter and the letter != c
        ((tree? T) ;if T is a tree
         (string-append "0" (huff-encode-single c (tree-left T)));traverse left subtree
         (string-append "1" (huff-encode-single c (tree-right T)));traverse right subtree
         )
         )
  )
;test
;(define my-hoff (hoff-tree (sort (produce-pairs frequency letters) 0)))
;(huff-encode-single "a" my-hoff)
;>"111" it just returns this same sequence no matter what I input, no idea why

;takes in huffman tree, returns all the characters and their encodings as a list of pairs
(define (read-hoff T letters)
  (if (null? letters)
      '()
      ;runs huff-encode single on every character in 'letters'
      (cons (cons (car letters) (huff-encode-single (car letters) T)) (read-hoff T (cdr letters)))
      )
  )
;test
;(define my-hoff (hoff-tree (sort (produce-pairs frequency letters) 0)))
;(read-hoff my-hoff letters)
;>(("a" . "111")("b" . "111")("c" . "111")...and so on

;Assignment 3

;runs huff-encode-single on each character in the msg
(define (encode msg T)
  (if (null? msg)
      ""
      (string-append (huff-encode-single (car msg) T)
              (encode (cdr msg) T))))
;test
;(define my-hoff (hoff-tree (sort (produce-pairs frequency letters) 0)))
;(encode '("c" "a" "t") my-hoff)
;>"111111111" <-due to the problem I had with huff-encode-single only returning "111" for anything i input

;Assignment 4

;takes in a 0/1 sequence and returns decoded string using the given huffman tree
(define result "");empty string that the decode procedure will keep adding to
(define (decode-h sequence T)
  (cond ((letter? T) (set! result (string-append result (tree-left T))))
        ((equal? "0" (car sequence)) (decode (cdr sequence) (tree-left T)));if the next bit in the sequence is a 0, traverse the left subtree
        ((equal? "1" (car sequence)) (decode (cdr sequence) (tree-right T)));if the next bit in the sequence is a 1, traverse the right subtree
      )
  )
;test
;(define my-hoff (hoff-tree (sort (produce-pairs frequency letters) 0)))
;(decode '("0" "1" "1" "1") my-hoff)
;(display result) ;>n
;(decode '("0" "0" "1" "0") my-hoff)
;(display result) ;>r
;(decode '("1" "1" "1" "0") my-hoff)
;(display result) ;>SPACE


;Assignment 5

;huff object
(define (H-code-object lets freq)
  (let ((letters lets) (frequency freq) (T '()))
    
    ;procedure that takes the above lists and produces a list of trees: node is weight, left is char, right is null
     (define (produce-pairs freq chars) 
       (if (null? freq)
           '()
           (cons (list (car freq) (car chars) '()) (produce-pairs (cdr freq) (cdr chars)))
           )
       )

     ;returns frequency from a pair
     (define (value p) (car p))

     ;procedure that returns partially ordered list of pairs based on frequency 
     (define (helper-sort L)
       (cond ((null? L) '())
             ((= (length L) 1) L)
             ((<= (value (car L)) (value (list-ref L 1))) (append (list (car L)) (helper-sort (cdr L))))
             (else (helper-sort (append (cdr L) (cons (car L) '()))))
             )
       )

     ;runs helper-sort on the list (as many times as there are elements in the list) * 5 -not the most efficient but it seems to get the job done
     (define (sort L index) ;index starts as 0
       (if (= index (* 5 (length L)))
           L
           (sort (helper-sort L) (+ index 1))
           )
       )

     ;takes the list and returns it with first two elements removed
     (define (remove-first-two L i) ;i starts as 0
       (if (= i 2)
           L
           (remove-first-two (cdr L) (+ i 1))
           )
       )

     ;creates combined tree with the first two trees, the node being the combined weight, and the left and right being T1 and T2
     (define (sub-tree T1 T2)
       (list (+ (car T1) (car T2)) T1 T2)
       )

     ;"main" procedure for creating huff tree, goes through the ordered list of pairs. With each recursion,
     ;takes two of the least summed values, produces huffman tree,
     ;and replaces those used values with the new tree in the list, then recurses
     ;Once there is one element remaining in the list, just returned the combined hoff tree 
     (define (hoff-tree L)
       ;(display L)
       (if (= (length L) 1)
           (car L)
           ;pull the least two from list, combine them into tree
           ;recurse with the first two elements of L replaced by new-hoff-tree (and re-sorted)
           (hoff-tree (sort (cons (sub-tree (list-ref L 0) (list-ref L 1)) (remove-first-two L 0)) 0))
           )
       )

     ;Returns the value of the current root
     (define (tree-value T) (car T))

     ;Returns left subtree
     (define (tree-left T) (cadr T))

     ;Returns right subtree
     (define (tree-right T) (caddr T))

     ;to check is a tree is a letter, the right element will be null
     (define (letter? T) (null? (tree-right T)))

     ;determines if the given tree is a hoffman tree vs just a letter
     (define (tree? T)
       (if (eq? #t (letter? T))
           #f
           #t)
       )

     ;takes in a character as an input, returns its encoding
     (define (huff-encode-single c T)
       (cond ((and (letter? T) (equal? c (tree-left T))) c) ;if T is a letter and the letter == c
             ((and (letter? T) (not (equal? c (tree-left T)))) "") ;if T is a letter and the letter != c
             ((tree? T) ;if T is a tree
              (string-append "0" (huff-encode-single c (tree-left T)));traverse left subtree
              (string-append "1" (huff-encode-single c (tree-right T)));traverse right subtree
              )
             )
       )

     ;runs huff-encode-single on each character in the msg
     (define (encode msg T)
       (if (null? msg)
           ""
           (string-append (huff-encode-single (car msg) T)
                          (encode (cdr msg) T))))

    ;decoder
     (define (decode msg T)
       ;takes in a 0/1 sequence and returns decoded string using the given huffman tree
       (define result "");empty string that the decode procedure will keep adding to
       (define (decode-h sequence T)
         (cond ((letter? T) (set! result (string-append result (tree-left T))))
               ((equal? "0" (car sequence)) (decode-h (cdr sequence) (tree-left T)));if the next bit in the sequence is a 0, traverse the left subtree
               ((equal? "1" (car sequence)) (decode-h (cdr sequence) (tree-right T)));if the next bit in the sequence is a 1, traverse the right subtree
               )
         )
       (decode-h msg T)
       result
       )

     ;takes in huffman tree, returns all the characters and their encodings as a list of pairs
     (define (gen-table T letters)
       (if (null? letters)
           '()
           ;runs huff-encode single on every character in 'letters'
           (cons (cons (car letters) (huff-encode-single (car letters) T)) (gen-table T (cdr letters)))
           )
      )

    ;set the internal T attribute to a hoffman tree based off letters and frequencies passed
    (set! T (produce-pairs letters frequency))
    ;dispatcher
    (lambda (method)
      (cond ((eq? method 'encode) encode)
            ((eq? method 'decode) decode)
            )
      )
    )
  )
;when I run the script, even without inititiating any objects from the above definition, it never terminates and just severely lags racket, eating up tons
;of memory























