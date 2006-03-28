import sys, re, os
from string import Template

# parse command line
source = sys.argv[1]
options = {}
parser = re.compile( '([^=]+)=(.+)' )
for x in sys.argv[2:] :
    m = parser.match( x );
    if m : options[m.group(1)] = m.group(2)

# substitude string template
fp = open(source)
templ = Template( fp.read() )
fp.close()
str = templ.substitute( options );
print( str )
