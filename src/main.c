#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static const char *program_version = "0.631.0";
static const char default_banner[] =
    "  ____       _         ____ _     ___ \n"
    " / ___| __ _| |_ ___  / ___| |   |_ _|\n"
    "| |  _ / _` | __/ _ \\| |   | |    | | \n"
    "| |_| | (_| | || (_) | |___| |___ | | \n"
    " \\____|\\__,_|\\__\\___/ \\____|_____|___|\n"
    "                                      \n";

static void print_usage(const char *prog) {
  printf("Usage: %s <command> [options]\n", prog);
  puts("Commands:");
  puts("  greet [name]                 Print a friendly greeting (default: "
       "there)");
  puts("  repeat [count]               Print a numbered notification multiple "
       "times");
  puts("  banner [file]                Print the contents of a banner file "
       "(default: banner.txt)");
  puts("  hello [count]                Repeats hello multiple times");
  puts("  add [num1] [num2]            Adds two integers");
  puts("  echo [sentence]              Repeats anything you say");
  puts("  count [sentence] [character] Counts the occurrences of a certain "
       "character in a given sentence");
  puts("  test [unspecified]           A testing command for developers");
  puts("  --version, -v                Show version information");
  puts("  --help, -h                   Show this help message");
}

static void print_version(const char *prog) {
  printf("%s version %s\n", prog, program_version);
}

static void greet(const char *name) { printf("Hello, %s!\n", name); }

static void repeat_notification(int count) {
  for (int i = 1; i <= count; ++i) {
    // show each notification with its number so the caller can tell how many
    // happened.
    printf("%d: This is notification #%d\n", i, i);
  }
}

static void repeat_hello(int count) {
  for (int i = 1; i <= count; i++) {
    // just mirror the loop above but stick with a cheerful hello message.
    printf("Hello! This is hello number: #%d\n", i);
  }
}

// TODO: maybe use this later?
char *get_input(int allocatedBytes) {
  char *buffer = malloc(allocatedBytes);
  fgets(buffer, allocatedBytes, stdin);
  return buffer;
}

static int count_occurrences(char string[], char occurrence) {

  // this gets the size of the array because sizeof() returns the size of the
  // array in bytes, and it divides that by the size of one element (in bytes)
  // int length = sizeof(string) / sizeof(string[0]);

  // but we can just use strlen()
  int length = strlen(string);

  int count = 0;

  for (int i = 0; i < length; i++) {
    if (string[i] == occurrence) {
      count++;
    }
  }

  // printf("The length of the string was: %d\nAnd '%c' occurred %d times.",
  // length, occurrence, count);
  return count;
}

static int parse_positive_int(const char *text) {
  if (!text) {
    return 0;
  }

  // only give callers plain positive numbers so they don't have to check.
  errno = 0;
  char *endptr = NULL;
  long value = strtol(text, &endptr, 10);

  if (errno != 0 || *endptr != '\0' || value <= 0 || value > INT_MAX) {
    return -1;
  }

  return (int)value;
}

static int add_integers(const int num1, const int num2) { return num1 + num2; }

static int coin_flip() { return rand() % 2; }

static char *read_file_to_string(const char *path) {
  FILE *file = fopen(path, "rb");
  if (!file) {
    fprintf(stderr, "error: unable to open '%s': %s\n", path, strerror(errno));
    return NULL;
  }

  if (fseek(file, 0, SEEK_END) != 0) {
    fprintf(stderr, "error: seeking '%s': %s\n", path, strerror(errno));
    fclose(file);
    return NULL;
  }

  long size = ftell(file);
  // size tells us how many bytes we need to read.
  if (size < 0) {
    fprintf(stderr, "error: measuring '%s': %s\n", path, strerror(errno));
    fclose(file);
    return NULL;
  }

  if (fseek(file, 0, SEEK_SET) != 0) {
    fprintf(stderr, "error: rewinding '%s': %s\n", path, strerror(errno));
    fclose(file);
    return NULL;
  }

  size_t length = (size_t)size;
  char *buffer = malloc(length + 1);
  if (!buffer) {
    fprintf(stderr, "error: out of memory reading '%s'\n", path);
    fclose(file);
    return NULL;
  }

  size_t read_bytes = fread(buffer, 1, length, file);
  if (read_bytes != length && ferror(file)) {
    fprintf(stderr, "error: reading '%s': %s\n", path, strerror(errno));
    free(buffer);
    fclose(file);
    return NULL;
  }

  buffer[read_bytes] = '\0';
  // mark the end so the buffer is a proper C string.
  fclose(file);
  return buffer;
}

static void print_banner(const char *path) {
  char *text = read_file_to_string(path);
  if (!text) {
    return;
  }

  puts(text);
  free(text);
}

static void print_default_banner(void) { puts(default_banner); }

int main(int argc, char *argv[]) {
  // drop the banner up front so the CLI always starts with some flair.
  print_default_banner();

  // init the random thing? pass the current time as a seed i assume
  srand(time(NULL));

  const char *prog = argc > 0 ? argv[0] : "cli";

  if (argc < 2) {
    print_usage(prog);
    return 0;
  }

  const char *command = argv[1];

  if (strcmp(command, "--help") == 0 || strcmp(command, "-h") == 0) {
    print_usage(prog);
    return 0;
  }

  if (strcmp(command, "add") == 0) {
    int result;
    if (argc >= 4) {
      result = add_integers(parse_positive_int(argv[2]),
                            parse_positive_int(argv[3]));
    } else {
      fprintf(stderr, "Please put at least 2 arguments for this option\n");
      return 1;
    }
    printf("The result of the operation was: %d\n", result);
    return 0;
  }

  if (strcmp(command, "--version") == 0 || strcmp(command, "-v") == 0) {
    print_version(prog);
    return 0;
  }

  if (strcmp(command, "greet") == 0) {
    const char *name = argc >= 3 ? argv[2] : "there";
    greet(name);
    return 0;
  }

  if (strcmp(command, "repeat") == 0) {
    int count = 1;
    if (argc >= 3) {
      int parsed = parse_positive_int(argv[2]);
      if (parsed <= 0) {
        fprintf(stderr, "error: repetition count must be a positive integer\n");
        return 1;
      }
      count = parsed;
    } else {
      fprintf(stderr, "Please put at least 1 argument for this option.\n");
      return 1;
    }
    repeat_notification(count);
    return 0;
  }

  if (strcmp(command, "banner") == 0) {
    const char *path = argc >= 3 ? argv[2] : "banner.txt";
    print_banner(path);
    return 0;
  }

  if (strcmp(command, "hello") == 0) {
    if (argc >= 3) {
      int parsed = parse_positive_int(argv[2]);
      if (parsed <= 0) {
        fprintf(stderr,
                "error: repetition count must be a positive integer!\n");
        return 1;
      }
      repeat_hello(parsed);
    } else {
      fprintf(stderr, "Please put at least 1 argument for this option.\n");
      return 1;
    }
    // fprintf(stderr, "This is just for testing and has not been fully
    // implemented yet! This will repeat %d times, despite your input of %d.",
    // times, parse_positive_int(argv[2]));
    return 0;
  }

  if (strcmp(command, "echo") == 0) {
    if (argc < 3) {
      fprintf(stderr, "Please put at least 2 arguments for this option "
                      "(including echo).\n");
      return 1;
    }

    for (int i = 2; i < argc; ++i) {
      printf("%s ", argv[i]);
    }
    printf("\n");
    return 0;
  }

  if (strcmp(command, "coinflip") == 0) {
      
    int result = coin_flip();
    int times = 1;

    if (argc >= 2) {
      times = parse_positive_int(argv[2]);
    }

    if (times < 1) {
      times = 1;
    }

    for (int i = 0; i < times; i++) {
      if (result == 0) {
        puts("Heads!");
      } else {
        puts("Tails!");
      }
    }
    return 0;
  }
  if (strcmp(command, "count") == 0) {
    // char first_string[] = argv[2];
    // char occurrence_to_check_for = argv[3];
    printf("There are %d occurrences of '%c'",
           count_occurrences(argv[2], *argv[3]), *argv[3]); // dereference argv3
    return 0;
  }

  fprintf(stderr, "error: unknown command '%s'\n", command);
  print_usage(prog);
  return 1;
}
