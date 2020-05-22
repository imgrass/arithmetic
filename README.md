Description: This directory is used for arithmetic problems:

## Some rules
There are some status of file to indicate process
    @v      completed and verified
    @i      in progress
    @p      request is given, but not yet started

## arithmetic problems
* sqrt
    file: sqrt.c
    status: @v
    summary: To get the extraction of a root, do not use lib function.
    make option:
        make sqrt
        make sqrt debug=true
        make sqrt_ut
