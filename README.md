# PIPEX (42)

*Who knew pipes could read and write?*<br/>
*Better than some people apparantly...*

## Subject

Your objective is to code the Pipex program.<br/>
It should be executed in this way:<br/>``$> ./pipex file1 cmd1 cmd2 file2``<br/>Just in case:<br/>file1 and file2 are file names, cmd1 and cmd2 are shell commands with their parameters.<br/>The execution of the pipex program should do the same as the next shell command:<br/>``$> < file1 cmd1 | cmd2 > file2``

### Mandatory

``$> ./pipex infile "ls -l" "wc -l" outfile`` <br/>should be the same as<br/> ``< infile ls -l | wc -l > outfile``<br/><br/>
``$> ./pipex infile "grep a1" "wc -w" outfile`` <br/>should be the same as<br/> ``< infile grep a1 | wc -w > outfile``

### Bonus

1. Handle multiple pipes :<br/>
``$> ./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2``<br/>
Must be equivalent to :<br/>
``< file1 cmd1 | cmd2 | cmd3 ... | cmdn > file2``<br/>
2. Support « and » when the first parameter is "here_doc"<br/>
``$> ./pipex here_doc LIMITER cmd cmd1 file``<br/>
Must be equivalent to:<br/>
``cmd << LIMITER | cmd1 >> file``<br/>

### Observations

1. The ``main`` function has a third parameter: ``char **envp``

2. ``execve`` requires the actual path to the binary of the command you want to execute.<br/>
If only there was an ``environment variable`` that holds **almost** all the ``PATH``s.

3. In my implementation of the bonus I create a temporary file which I delete after the input is no longer needed, this can be replaced by just ``piping`` the input directly to the command, get creative!

4. Using one pipe only is sexy. Period.<br/>
But maybe a bit harder to read, or is it write?

5. Small oversight: outfile gets created even if the program fails...<br/>
Now, how can we solve that?<br/>
Maybe there is a function that can take care of that...
