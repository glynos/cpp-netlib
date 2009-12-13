#!/bin/env python

from sys import argv
import httplib2 as httplib
from subprocess import Popen,PIPE

if len(argv) < 3:
    print('I need the executable to run.')
    exit(1)

print('Running {0}...'.format(argv[1]))

pipe = None
try:
    pipe = Popen(argv[1], executable=argv[1], stdin=PIPE, stdout=PIPE, close_fds=True)
    print('Done with spawning {0}.'.format(argv[1]))
except:
    print('I cannot spawn \'{0}\' properly.'.format(argv[1]))
    exit(1)

status = 0
client = httplib.Http(timeout=5)
expected = b'Hello, World!'

def test(url, method, expected, headers={}, body=''):
    resp, content = client.request(url, method, headers=headers, body=body)
    print('Request: {method} {url} body=\'{body}\''.format(method=method, url=url, body=body)),
    if content != expected:
        print('ERROR: \'{0}\' != \'{1}\''.format(content, expected))
        status = 1
    else:
        print('... passed.')

def test_status(url, method, expected, headers={}, body=''):
    resp, content = client.request('http://localhost:8000/', 'PUT', body='')
    print('Request: {method} {url} body=\'{body}\''.format(method=method, url=url, body=body)),
    if resp['status'] != expected:
        print('ERROR: response status ({0}) != 400'.format(resp['status']))
        status = 1
    else:
        print('... passed.')

url = 'http://localhost:8000/'
test(url, 'GET', expected)
test(url, 'DELETE', expected)
# Good request case, there's a content-length header for POST
test(url, 'POST', expected, {'Content-Length': '0'})
# Good request case, there's a content-length header for PUT
test(url, 'PUT', expected, {'Content-Length': '0'})
# Bad request case, no content-length for POST
test_status(url, 'POST', '400')
# Bad request case, no content-length for PUT
test_status(url, 'PUT', '400')

if status != 0:
    print('Failures encountered.')
    pipe.kill()
    exit(status)

open(argv[len(argv) - 1], 'w').close()
print('All tests pass.')
pipe.kill()

