# Complier for C
* Thank you! [study-aid site](https://www.sigbus.info/compilerbook)

## Docker Set up
```
docker build . -t "9cc"
```

## Allocate Volume and Enter Continer for the First Time
```
docker run -it --cap-add=SYS_PTRACE --security-opt="seccomp=unconfined" --name "9cc" -v $(pwd):/9cc:delegated 9cc /bin/bash
```

## Enter Container
```
docker exec -it <continer-id> /bin/bash
```

## Circle CI Test in local
```
circleci local execute --job test
```
