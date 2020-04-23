from argparse import ArgumentParser
from pathlib import Path
from termcolor import colored

import subprocess as sp
import yaml

tests_file = 'tests/tests.yaml'

def run_shell(args, cmd):
    return sp.run(args, capture_output=True, text=True, input=cmd)

def test(binary, test_case):
    ref = run_shell(['bash', '--posix'], test_case['stdin'])
    our_shell = run_shell([binary], test_case['stdin'])

    for check in test_case.get('checks', ['stdout', 'stderr', 'returncode', 'has_stderr']):
        if check == 'stdout':
            assert ref.stdout == our_shell.stdout
        elif check == 'stderr':
            assert ref.stderr == our_shell.stderr
        elif check == 'returncode':
            assert ref.returncode == our_shell.returncode
        elif check == 'has_stderr':
            assert our_shell.stderr != ''

def get_nb_tabs(str):
    return '\t\t' if len(str) < 9 else '\t'

def check_flag_c_conditions(flag_c, flag_c_descriptions, description):
    return flag_c and description in flag_c_descriptions

if __name__ == "__main__":
    parser = ArgumentParser(description="Our Testsuite")
    parser.add_argument('-l', '--list', action='store_true', dest='flag_l')
    parser.add_argument('bin', metavar='BIN')
    parser.add_argument('-c', '--category', dest='flag_c', type=str, nargs='+')

    args = parser.parse_args()
    flag_c = args.flag_c
    flag_l = args.flag_l

    binary = Path(args.bin).absolute()

    with open(tests_file, 'r') as tests_file:
        content = yaml.safe_load(tests_file)

    for test_case in content:
        desc = test_case['description']
        debug_description = (desc + get_nb_tabs(desc)) if flag_l else ''
        should_print = check_flag_c_conditions(flag_c, args.flag_c, desc)
        try:
            test(binary, test_case)
        
        except AssertionError as err:
            if should_print:
                print(colored('[ KO ]', 'red'), debug_description, test_case['stdin']) 
        else:
            if should_print:
                print(colored('[ OK ]', 'green'), debug_description, test_case['stdin'])