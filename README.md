# SinglePassword

## Screenshot

![alt text](screen.jpg?raw=true)

## Build

Libraries:

- Qt 6.0.3
- LibreSSL 2.8.3 (`openssl version`)
- Boost 1.75.0_2


Make standalone for MacOS:

`macdeployqt SinglePassword.app`

## Database
The data is saved in your home directory, in the path: `$HOME/.singlePass/db.spdb`.
That is the file where all your secured data is stored.

The application is always using this file right now.

It is planned to improve application with possibility to point which file to use for secured data.
