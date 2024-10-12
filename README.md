# Eleca
Eleca is a simple command-line program to play with [elementary cellular automata](https://en.wikipedia.org/wiki/Elementary_cellular_automaton).

# Usage
```console
Options:
   -c, --column   {number}    set amount of cells horizontally (default 20)
   -r, --row      {number}    set amount of cells vertically (default 20)
   -R, --rule     {number}    set rule as integer (default 110)
   -s, --scale    {number}    set image scale to a positive integer
   -f, --file     {string}    set file name of extracted image
   -C, --centered             make first row centered
   -e, --extract              extract final state as .ppm image
   -S, --scroll               slowly print rows non-stop
   -h, --help                 see this message
```

![Rule 110 cellular automata, generated as .ppm with Eleca and converted to .png using an external program](res/demo.png)
