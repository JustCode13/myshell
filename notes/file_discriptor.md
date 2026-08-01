# Now what is file discriptor (FD)

I hope i spell it right 

FD is nothing but a signed integer 

Every open file and input, output stream in a process get a FD 
just so can we can identify them, the standard FDs are 
stdin = 0, stdout = 1, stderr = 2 
stdin is the terminal keyboard to get the input,
stdout is the terminal to display the output,
stderr is the terminal to displayy the error

now like that when you open files they also get FD's 

example

int fd = open(filename,readonly)

so in a nutshell, FD is just a signed integer given to identify them

what is a process

process is something which is currently running

like when you write a program and when you run it 
you say to os like "hey, run the program"
and when it runs the program that is called process

you can run the process as many times as you want  

./program
./program 
./program

didn't wanna brag about it, but i wrote this myself.
not fucking goddamn AI at all
