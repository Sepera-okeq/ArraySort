from random_word import RandomWords
import random
import sys
import argparse
import os
import string

class TextFileGenerator():
    def __init__(self, number_of_files, file_length, word_list, file_name, start_number, ran_file_name):
        self.random_generator = RandomWords()
        self.number_of_files = int(number_of_files)
        self.file_length = int(file_length)
        self.word_list = word_list
        self.current_random_words = None
        self.ran_file_name = ran_file_name
        if not start_number == None:
            self.start_number = start_number
        else:
            self.start_number = 0
        if not file_name == None:
            self.file_name = file_name
        else:
            self.file_name = 'testing_file'
        self.output_folder()

    def get_random_words_list(self):
        self.current_random_words = self.random_generator.get_random_words()
        while self.current_random_words == None:
            self.current_random_words = self.random_generator.get_random_words()

    def generate_file(self, name):
        self.get_random_words_list()
        with open(name, 'w') as f:
            for i in range(self.file_length):
                currentLine = ''
                for x in range(random.randrange(15, 30, 1)):
                    if ((random.randrange(1, 5, 1)) == 1) and not (self.word_list == None):
                        currentLine += f'{random.choice(self.word_list)} '
                    else: 
                        currentLine += f'{random.choice(self.current_random_words)} '
                f.write(f'{currentLine}\n')

    def generator(self):
        for n in range(self.number_of_files):
            if self.ran_file_name:
                name = f'Output/{self.random_file_name()}.txt'
            else:
                name = f'Output/{self.file_name}{n + int(self.start_number)}.txt'
            self.generate_file(name)

    def random_file_name(self):
        letters = string.ascii_letters
        return (''.join(random.choice(letters) for i in range(10)))

    def output_folder(self):
        if not os.path.isdir('Output'):
            os.mkdir('Output')

def parse_arguments():
    parser = argparse.ArgumentParser(description="Генерирует N файлов с L строками, используя случайные полные слова.  Можно включить список слов, если вам нужно, чтобы определенные слова были включены случайным образом.")
    parser.add_argument('Number of files', metavar='N', type=int, help="Number of files to create")
    parser.add_argument('Number of lines', metavar='L', type=int, help="Number of lines to add to each file")
    parser.add_argument('-w', '--word-list', metavar='[TEXT]', nargs='*', help="Word list should you want specific words included")
    parser.add_argument('-s', '--start-number', metavar='[TEXT]', help="File number to start with.")
    parser.add_argument('-n', '--file-name', metavar='[TEXT]', help="Name used to generate files.")
    parser.add_argument('-r', '--random-file-name', action='store_true', help="Instead of uniform filenames, use a random filename each time.")

    args = parser.parse_args()
    return args

def main():
    args = vars(parse_arguments())
    generator = TextFileGenerator(args['Number of files'], args['Number of lines'], args['word_list'], args['file_name'], args['start_number'], args['random_file_name'])
    generator.generator()

if __name__ == '__main__':
    main()