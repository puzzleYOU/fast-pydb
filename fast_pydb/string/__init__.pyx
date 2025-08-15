cdef extern from "clean_string.h":
    void cleanString(const char *source,
                     char *destination,
                     const unsigned int size)

def clean_string(source: str | None) -> str | None:
    """
    Cleans the source string according following rules:
    - redundant spaces
    - trailing and leading whitespaces
    - trailing and leading newlines

    >>> clean_string() 
    Traceback (most recent call last):
    ...
    TypeError: function takes exactly 1 argument (0 given)
    >>> clean_string(1) 
    Traceback (most recent call last):
    ...
    TypeError: a bytes-like object is required, not 'int'
    >>> clean_string(1, 2, 3)
    Traceback (most recent call last):
    ...
    TypeError: function takes exactly 1 argument (3 given)
    >>> clean_string(None) 
    >>> clean_string('')
    ''
    >>> clean_string('   ')
    ''
    >>> clean_string('abc')
    'abc'
    >>> clean_string('   abc') 
    'abc'
    >>> clean_string('abc    ') 
    'abc'
    >>> clean_string('   abc    ') 
    'abc'
    >>> clean_string('   a b c    ')
    'a b c'
    >>> clean_string('   a    b c    ')
    'a b c'
    >>> clean_string('   a    b       c    ')
    'a b c'
    >>> clean_string('   \n\n  a\n\nb\n\nc  \n  ')
    'a\n\nb\n\nc'
    """

    if source is None:
        return None
    
    buffer = ""
    cleanString(source.encode(), buffer.encode(), len(source))
    return buffer