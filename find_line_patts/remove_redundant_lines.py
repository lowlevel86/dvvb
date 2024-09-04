#!/bin/python3

def remove_redundant_lines(input_file, output_file):

    with open(input_file, 'r') as file:
        lines = file.readlines()

    # create a set to store unique lines
    unique_lines = set()

    # write the unique lines to the output file
    with open(output_file, 'w') as file:
        for line in lines:
            if line.strip() not in unique_lines:
                file.write(line)
                unique_lines.add(line.strip())

remove_redundant_lines('patts.txt', 'patts_rm.txt')
