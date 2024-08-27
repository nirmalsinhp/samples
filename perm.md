# permutations & Combinations
## permutations
- permutations
  - ways to arrange n. i.e seat 5 people in 5 seats.
  - it is n! permutations.
  - how many ways to seat 5 people in 3 chairs. n people r chairs -- n! / (n -r)!
  - if repeat is allowed, let's say 3 word letters from lower case alphabets - 26 ^ 3.
  - if repeat is not allowed, we do permutations. 26! / 23!
  - nPr = n! / (n - r)! 
  - nPn = n! -- 0! --> 1
  - order matters in permutations.


## combinations
    - how many ways we can arrange 6 people in 3 chairs --  permutations.
    - how many ways we can choose 3 people from 6 to seat in 3 chairs, order does not matter.
    - total number of permutations, divide by number of ways to arrange.
    - nCr(nChooser) - n! / r! (n - r)! -- binomial co-efficents 


- remember always check for nulls, empty container, and overflow boundry conditions to be safe.
- consider using stack/recusersion when reverse order is in picture.

# matrix
an array convert to n * m matrix => a[x] =>matrix[x / m][x % m];

## 2 stack algo for algorithmic expression evalution
- remember to use last pushed arg as first arg, as it is stack.

above works for infix, 
if postfix/reverse polish notation used, you can just use 1 stack and pop whenever operator occurs.

# union find -- check after/with graph

# under stand and think about, open-closed ranges, whether < last or <= last based on that.

# Rank : 

The heart of any algorithm is an idea. If your idea is
not clearly revealed when you express an algorithm, then you are using too
low-level a notation to describe it.