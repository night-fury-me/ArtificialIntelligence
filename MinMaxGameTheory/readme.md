### MinMax Game Theory with Alpha Beta Pruning

This is a simple implementation of MaxConnect4 Game using MinMax Game Theory with Alpha Beta Pruning in **C++**,

#### Compilation instruction:  ( For Linux / OSX )

**1.** Compile and Run with these below commands:
( Assuming current directroy is the directory where the maxConnect4.cpp file is, if not then goto section "2" )

```bash
~ g++ maxConnect4.cpp -o maxConnect4
```

#### For Interactive Game mode.
```bash
~ ./maxConnect4 maxconnect4 interactive [input_file] [computer-next/human-next] [depth]
```

#### For one-move mode
```bash
~ ./maxConnect4 maxconnect4 one-move [input_file] [output_file] [depth]
```

**2.** To check current Directory Use this command :
```bash
~ pwd
```
Change the directory to Decision Tree :

```bash
~ cd MINMAX_DIRECTORIES_PATH
```

Example:
```bash
~ cd /home/USER/AI/MinMaxGameTheory
```
Now Go back to section "**1**"



**Compiler version**
Some header files requires **C++11** or higher version of gcc compiler. ( **e.g.** unordered_map, utility, vector, queue, tuple )

These are only available in C++11 compilation mode, i.e. -std=c++11 or -std=gnu++11.
