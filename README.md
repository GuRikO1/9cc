# Complier for C
* Thank you! [study-aid site](https://www.sigbus.info/compilerbook)

## Docker Set up
```
docker build . -t "9cc"
```

## Allocate Volume and Enter Continer for the First Time
```
docker run -it --name "9cc" -v $(pwd):/9cc 9cc /bin/sh
```

## Enter Container
```
docker exec -it <continer-id> /bin/sh
```

## Circle CI Test in local
```
circleci local execute
```
