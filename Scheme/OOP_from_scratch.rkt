;assignment 1:
(define (basic-set)
  (let ((set-L '()))
    (define (empty?) (eq? set-L '()))
    (define (insert v) (set! set-L (cons v set-L)))
    (define (delete v)
      (define (remove v L) ;L starts off as copy of orig-L
        (cons ((null? L) '())
              ((eq? v (car L)) (cdr L))
              ;(else (cons (car L) (remove v (cdr L)))) causes the error: 'else: not allowed as an expression'
              )
        )
      (set! set-L (remove v set-L))
      )
    (define (element? v)
      (define (helper L v)
        (cond ((eq? L '()) #f)
              ((eq? (car L) v) #t)
              (else (helper (cdr L) v)))
        )
      (helper set-L v)
      )
    (lambda (method)
      (cond ((eq? method 'empty?) empty?)
            ((eq? method 'insert) insert)
            ((eq? method 'delete) delete)
            ((eq? method 'element?) element?)
            )
      )
    )
  )
;test
;(define test (basic-set))
;((test 'empty?))
;((test 'insert) 4)
;((test 'empty?))
;> #t #f

;assignment 2:
(define (stat-set)
  (let ((set-L '()) (curr-largest 0) (curr-smallest 0) (curr-length 0) (curr-average 0)) ;all start off as 0
    (define (empty?) (eq? set-L '()))
    (define (insert v)
      (cond ((> v curr-largest) (set! curr-largest v))
            ((< v curr-smallest) (set! curr-smallest v))
            )
      (set! curr-length (+ 1 curr-length))
      (set! set-L (cons v set-L))
      (set! curr-average (/ (apply + set-L) curr-length))
      )
    (define (element? v)
      (define (helper L v)
        (cond ((eq? L '()) #f)
              ((eq? (car L) v) #t)
              (else (helper (cdr L) v)))
        )
      (helper set-L v)
      )
    (define (largest) curr-largest)
    (define (smallest) curr-smallest)
    (define (size) curr-length)
    (define (average) curr-average)
    (lambda (method)
      (cond ((eq? method 'empty?) empty?)
            ((eq? method 'insert) insert)
            ((eq? method 'element?) element?)
            ((eq? method 'largest) largest)
            ((eq? method 'smallest) smallest)
            ((eq? method 'size) size)
            ((eq? method 'average) average)
            )
      )
    )
  )
;test
;(define test (stat-set))
;((test 'empty?))
;((test 'insert) 4)
;((test 'insert) 5)
;((test 'insert) 6)
;((test 'empty?))
;((test 'average))
;((test 'largest))
;>t
;>f
;>5
;>6

;assignment 3:
(define (multi-set)
  (let ((set-L '()))
    (define (empty?) (eq? set-L '()))
    (define (insert v)
      (define (iter-helper v L index) ;index should start at 0
        (cond ((eq? L '()) (set! set-L (cons (cons v 1) set-L))) ;if it gets to end of list, create new pair in list with the value and 1
              ((eq? (car (car L)) v) ;if the current pair matches the value, update that pair 
               (define (replace-index L prev-L v i) ;returns list with updated pair
                 (if (eq? (car (list-ref L index)) v) ;if the current pair matches the value
                     ;(append old-L (cons new-P (cdr L)))
                     (append prev-L (cons (cons v (+ 1 (cdr (list-ref L index)))) (cdr L))) ;(car L should be the matched pair)
                     (replace-index (cdr L) (cons (car L) L) v i)
                     )
                 )
               (set! set-L (replace-index set-L '() v index)))
              (else (iter-helper v (cdr L) (+ 1 index))))
        )
        ;if list is null, just add a new pair, else use helper to iterate through pairs 
        (if (eq? set-L '())
            (set! set-L (list (cons v 1)))
            (iter-helper v set-L 0)
            )
      )
    (define (multiplicity? v)
      (define (helper-iter1 L v)
        (cond ((eq? (caar L) v) (cadr L))
              ((eq? L '()) 0)
              (else helper-iter1 (cdr L) v)
            ))
      (helper-iter1 set-L v)
      )
    (define (delete-all v)
      (define (help-iter2 L v) 
        (cond ((eq? L '()) '()) ;if v not present, returns null
              ((eq? v (caar L)) (cdr L))
              (else (append (car L) (helper-iter2 (cdr L) v) )))
        )
      (if (eq? '() (helper-iter2 set-L v))
          0
          (set! set-L (helper-iter2 set-L v))
          )
      )
    (define (delete v)
      (define (help-iter3 L v) 
        (cond ((eq? L '()) '()) ;if v not present, returns null
              ((eq? v (caar L)) (cons (cons v (- (cadr L) 1)) (cdr L)))
              (else (cons (car L) (helper-iter3 (cdr L) v))))
        )
      (if (eq? '() (helper-iter3 set-L v))
          0
          (set! set-L (helper-iter3 set-L v))
          )
      )
    (lambda (method)
      (cond ((eq? method 'empty?) empty?)
            ((eq? method 'insert) insert)
            ((eq? method 'multiplicity?) multiplicity?)
            ((eq? method 'delete) delete)
            ((eq? method 'delete-all) delete-all)))
      )
  )































































































