# CS210 — SHELL PROJECT

## Introduction
Hello, welcome to our Shell repo! You can have a read below to reinforce some basic Git concepts if you're new to Git in general, if you're comfortable then you can omit the instructions. At the very bottom you also have a link to the PDFs necessary for the assingment so you won't have to hop between MyPlace and this repository to find what you need. Adios.

## Conventions

We will be adhering to the [kernel](https://www.kernel.org/doc/html/v4.10/process/coding-style.html) conventions with regards to Indendation, Comments, Functions and the likes, to stay consistent + I assume it will be a part of the marking scheme. The two things that we will not follow from the conventions is braces and indentation depth, see below for what we expect:

```
void print_numbers() {
    for (int i = 0; i < 11; i++) {
        printf(i + "\n");
    }
}
```

Basically opening curly brace of a function should be on the same line as the function declaration and the indentation should be one tab character (same length as 4 spaces).

## Github + Git
Given that some of you may be unfamiliar with how Git works, see below commands that you will be using often, starting from now.

Since this is of course a group project, we need to ensure that there won't be any issues with the codebase - so we can't just all push to master. We will use branches. It will be a little bit difficult to summarize in a sentence or two, so I recommend watching some kind of tutorial, like this [one](https://www.youtube.com/watch?v=e2IbNHi4uCI) (though there's shorter ones available).

- "git add ."

The most basic command in git, it adds new or changed files in your working directory to the Git staging area, with the dot at the end signifying to add every single item that has been added/changed.

- "git commit -m"

Another command you will have to use before pushing to the repository. Git commit creates a commit, which is like a snapshot of your repository. The way to use this command is to add a comment after the 'm', such as "git commit -m "this is a commit"".

- "git push -u origin master"

Given that you have initialized your repository and called the remote "master", you can use this command to push your local commits to the repository we can all see.

- "git pull"

This command updates your current local working branch with the changes that have been made by other people.

If you need any help with navigating the git space, feel free to contact Kamil, or Angus, or have a peep [here](https://github.com/git-guides/) which contains all the information you need.

## Additional Information

Click on the hyperlinks below to access whatever resource you may need.

[main shell pdf](https://github.com/AngusLogan02/flash/blob/master/myplace-pdfs/SimpleShell.pdf)\
[main shell testing pdf](https://github.com/AngusLogan02/flash/blob/master/myplace-pdfs/SimpleShellTests.pdf)\
[trello](https://trello.com/b/0BASllzp/flash-board)
