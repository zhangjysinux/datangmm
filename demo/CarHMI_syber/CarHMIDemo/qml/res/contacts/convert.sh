#!/bin/bash

sed 's/@/%/g' $1 |
awk '
BEGIN {
    printf "var addressBook =\n"
    print "["
    FS=";"
}
{
    print "{"
    printf ("\t\"name\": \"%s\",\n", $1)
    printf ("\t\"city\": \"%s\",\n", $2)
    printf ("\t\"phone\": \"%s\",\n", $3)
    printf ("\t\"email\": \"%s\"\n", $4)
    printf "},"
}
END {
    print "]"
    print "§§§"
}
' | sed 's/},{/},\n{/g' | sed 's/},]/}\n]/g' | sed 's/%/@/g' | sed 's/\n//g' | sed 's/"/\\"/g' | sed 's/§§§/"/g' |
awk '
    {
    if (NR > 1)
        printf ("\042%s\042 +\n", $0);
        else
        print $0
    }
' | sed 's/\"\"\" +//g' | sed 's/\"]\" +/\"]\"/g'
