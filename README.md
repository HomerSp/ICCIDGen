# ICCIDGen
Generates euimid from iccids

Usage: iccidgen <iccid list> [output csv]
    <iccid list> is a text file with one iccid per line
    [output csv] is optional and specifies where to write
        the output, in the format: ICCID;SF_EUIMID;EUIMID
        one per line.
        If this is not specified it will write to stdout.
