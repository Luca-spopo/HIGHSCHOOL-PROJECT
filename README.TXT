EDIT: This is from years ago, when I was a less knowledgable programmer.

Log in using:

Username:	r.sethi
Password:	sethi

Known bugs:

> puts function does not recognize the '\t' character as tab, and makes a circle instead.
> Sometimes text can appear deformatted and broken. There seem to be issues in strcpy. Since I do not know the internals of the string header, I cannot fix these. There is no recognizable pattern or method to bring these, and it happens randomly.
EDIT: Looking at it now, I think it is because of the scope of the variables. I may be returning pointers to deallocated strings. Will note for future projects.

Made by Anirudh Katoch
