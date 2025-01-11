# hunt3r

A command-line tool to check whether your passwords have been exposed in a data breach using an offline version of the HaveIBeenPwned database.

![Build Status](https://github.com/aliharbaji/haveibeenpwned-offline/actions/workflows/ci.yml/badge.svg)

## Overview

This tool enables you to check whether the passwords in your Bitwarden vault have been compromised by using the offline HaveIBeenPwned database. It operates locally without the need for online services, making it fast and efficient. The tool utilizes multiple threads to speed up the checking process.

The tool works by hashing each password using SHA-1, then truncating the first 5 characters of the hash to use as a filename. It then searches for the remaining part of the hash in the HaveIBeenPwned database to see if the password has been exposed in a data breach.

## Requirements

Before using the tool, you need to download the [_HaveIBeenPwned_](https://haveibeenpwned.com/) database and export your [_Bitwarden_](https://bitwarden.com/) vault in CSV format.

### Steps:

1. **Download the HaveIBeenPwned Database**

   To get the offline version of the [HaveIBeenPwned database](https://haveibeenpwned.com/Passwords), follow [Zsolt Müller's cURL approach](https://github.com/HaveIBeenPwned/PwnedPasswordsDownloader/issues/79) as described in the PwnedPasswordsDownloader issue #79.

2. **Export Your Bitwarden Vault**

    - Log into your Bitwarden account.
    - Export your vault in CSV format (this can be done from the [Bitwarden web vault](https://vault.bitwarden.com/)).

### Building the Project

1. Clone the repository:
   ```bash
   git clone https://github.com/aliharbaji/haveibeenpwned-offline.git
   cd haveibeenpwned-offline
   ```

2. **Install dependencies** and **build the project**:
   ```bash
   make
   ```

   This will generate an executable named `hunt3r`.

### Running the Tool

Once built, you can run the program to check your passwords against the offline HaveIBeenPwned database.

```bash
./hunt3r <path_to_csv_file> <path_to_pwned_db> <thread_pool_size = defaults to 8>
```

Where:
- `<path_to_csv_file>`: The full path to your **Bitwarden** exported CSV file.
- `<path_to_pwned_db>`: The full path to the downloaded **HaveIBeenPwned** database.
- `<thread_pool_size>`: Optional. The number of threads to use for processing. Defaults to `8` if not specified.

### Example Usage:

```bash
./hunt3r example_usage/bitwarden_example.csv example_usage/example_database 4                                                                                                                             
```


This command will check all the passwords in the example Bitwarden vault against the database in path example_usage/example_database using 4 threads.

### Output

The program will print a list of passwords that have been exposed in a data breach. Each exposed password will be printed with the number of breaches it has been part of.

```bash
   ___                             _           ___  _                  _    
  / _ \__      __ _ __    ___   __| |         / __\| |__    ___   ___ | | __
 / /_)/\ \ /\ / /| '_ \  / _ \ / _` | _____  / /   | '_ \  / _ \ / __|| |/ /
/ ___/  \ V  V / | | | ||  __/| (_| ||_____|/ /___ | | | ||  __/| (__ |   < 
\/       \_/\_/  |_| |_| \___| \__,_|       \____/ |_| |_| \___| \___||_|\_\

Successfully parsed and enqueued 1007 accounts.
PASSWORD 'password' PWNED -55- TIMES!
WE ADVISE YOU CHANGE IT ON THE FOLLOWING ACCOUNTS...(this mechanism is not yet supported)
PASSWORD 'password123' PWNED -99- TIMES!
WE ADVISE YOU CHANGE IT ON THE FOLLOWING ACCOUNTS...(this mechanism is not yet supported)
   __ _       _     _              _            _               _    
  / _(_)     (_)   | |            | |          | |             | |   
 | |_ _ _ __  _ ___| |__   ___  __| |______ ___| |__   ___  ___| | __
 |  _| | '_ \| / __| '_ \ / _ \/ _` |______/ __| '_ \ / _ \/ __| |/ /
 | | | | | | | \__ \ | | |  __/ (_| |     | (__| | | |  __/ (__|   < 
 |_| |_|_| |_|_|___/_| |_|\___|\__,_|      \___|_| |_|\___|\___|_|\_\    
```
## Features

- **Offline Checking**: Check if your passwords have been pwned without needing an internet connection.
- **Multithreading**: The tool supports multi-threaded checking for faster processing of large vaults.
- **CSV Input**: Supports the Bitwarden CSV format to import your vault easily.

## Troubleshooting

- If the tool does not find your CSV file or the pwned database, make sure that the paths provided are correct.
- If you see any issues with performance, try reducing the number of threads using the optional `<thread_pool_size>` parameter.

## Contributing

Feel free to fork this repository, open issues, or submit pull requests.

## License

This project is licensed under the MIT License - see the [LICENSE](../LICENSE) file for details.
