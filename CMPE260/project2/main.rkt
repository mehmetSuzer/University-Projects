; mehmet suzer
; 2019400213
; compiling: yes
; complete: yes

#lang racket


; 10 points
(define := (lambda (var value) ; walrus
             (list var value)))


; 10 points
(define -- (lambda args
             (list 'let args)))


; 10 points
(define @ (lambda (bindings expr)
            (list 'let (cadr bindings) (car expr))))


; 20 points
(define get_upto_delim (lambda (delim lis)  ; return a list which contains elements upto the delimeter given
            (cond ((null? lis) '())
                  ((eq? (car lis) delim) '())
                  (else (cons (car lis) (get_upto_delim delim (cdr lis)))))))

(define get_after_delim (lambda (delim lis) ; return a list which contains elements after the delimeter given
            (cond ((null? lis) '())
                  ((eq? (car lis) delim) (cdr lis))
                  (else (get_after_delim delim (cdr lis))))))

(define split_at_delim (lambda (delim args) ; splits list with respect to delimeter given
            (cond ((null? args) '())
                  (else (cons (get_upto_delim delim args) (split_at_delim delim (get_after_delim delim args)))))))


; 30 points
(define splitable_by (lambda (delim expr)  ; returns true if the expr can be split by the delimeter, otherwise, returns false
                       (if (null? (cdr (split_at_delim delim expr))) #f #t)))

(define parse_by_+ (lambda (expr)  ; parses an expr with respect to +
                   (if (member '+ expr) (cons '+ (map parse_expr (split_at_delim '+ expr))) expr)))

(define parse_by_* (lambda (expr)  ; parses an expr with respect to *
                   (if (member '* expr) (cons '* (map parse_expr (split_at_delim '* expr))) expr)))

(define convert_walrus (lambda (expr) ; ('x := 6) ---> (:= 'x 6)
                          (list (cadr expr) (car expr) (caddr expr))))

(define get_let_expr (lambda (expr)  ; ('x := 3 -- 'y := 6 --)  ---> '(let ((x 3) (y 6)))
                       (list 'let (map eval (map convert_walrus (split_at_delim '-- expr))))))

(define (parse_by_@ expr)  ; parses an expr with respect to @ 
  (let ((let_expr (get_let_expr (caar (split_at_delim '@ expr)))))
        (cons (car let_expr) (list (cadr let_expr) (parse_expr (caadr (split_at_delim '@ expr)))))))

(define (parse_expr expr)
  (cond ((and (null? (cdr expr)) (not (list? (car expr)))) (car expr))                      ; if expr = (x), return x
        ((and (null? (cdr expr)) (list? (car expr))) (parse_expr (car expr)))               ; else if expr ((...(x)...)) return x
        ((or (splitable_by '+ expr) (splitable_by '* expr)) (parse_by_* (parse_by_+ expr))) ; else if expr can be split by +, parse it by +, then by *
        ((eq? 'let (car expr)) expr)                                                        ; else if the first element is let, then there is nothing to parse
        (else (parse_expr (parse_by_@ expr)))))                                             ; else, there is a bounding, parse with respect to @
  

; 20 points
(define eval_expr (lambda (expr)
                   (eval (parse_expr expr))))







   