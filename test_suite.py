from argparse import ArgumentParser
from pathlib import Path
from termcolor import colored

import subprocess as sp
import yaml

tests_file = 'tests/tests.yaml'

def run_shell(args, cmd):
    return sp.run(args, capture_output=True, text=True, input=cmd)

def get_nb_tabs(str):
    return '\t\t' if len(str) < 9 else '\t'

def check_flag_c_conditions(flag_c, flag_c_descriptions, description):
    return not flag_c or description in flag_c_descriptions

def test(binary, test_case, debug_description):
    for i in range(1,len(test_case)):
        ref = run_shell(['bash', '--posix'], test_case[i]['stdin'])
        our_shell = run_shell([binary], test_case[i]['stdin'])

        for check in test_case[i].get('checks', ['stdout', 'stderr', 'returncode', 'has_stderr']):
            try:
                if check == 'stdout':
                    assert ref.stdout == our_shell.stdout
                elif check == 'stderr':
                    assert ref.stderr == our_shell.stderr
                elif check == 'returncode':
                    assert ref.returncode == our_shell.returncode
                elif check == 'has_stderr':
                    assert our_shell.stderr != ''
            except AssertionError:
                print(colored('[ KO ]', 'red'), debug_description, test_case[i]['stdin']) 
            else:
                print(colored('[ OK ]', 'green'), debug_description, test_case[i]['stdin'])

if __name__ == "__main__":
    parser = ArgumentParser(description="Our Testsuite")
    parser.add_argument('-l', '--list', dest='flag_l', action='store_true')
    parser.add_argument('bin', metavar='BIN')
    parser.add_argument('-c', '--category', dest='flag_c', type=str, nargs='+')

    args = parser.parse_args()
    flag_c = args.flag_c
    flag_l = args.flag_l

    binary = Path(args.bin).absolute()

    with open(tests_file, 'r') as tests_file:
        content = yaml.safe_load(tests_file)

    for test_case in content:
        desc = test_case['description'][0]['name']
        debug_description = (desc + get_nb_tabs(desc)) if flag_l else ''
        should_print = check_flag_c_conditions(flag_c, args.flag_c, desc)
        
        if should_print:
            test(binary, test_case['description'], debug_description)
            