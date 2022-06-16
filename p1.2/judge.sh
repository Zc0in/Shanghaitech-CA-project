#!/bin/bash

move_rubbish(){
    rm -rf translator.output
}

clean(){
    clear
    move_rubbish
}

judge (){
    echo "return nothing if succeed:"
    move_rubbish
    java -jar venus-jvm-latest.jar translator.S >> translator.output
    diff translator.output solution 
    
}

update(){
    move_rubbish
    git add .
    git commit -m "odd bugs"
    git pull
    source judge.sh
}

shpush(){
    move_rubbish
    git add judge.sh
    git commit -m "update shell"
    git push
}

push(){
    move_rubbish
    git add translator.S
    git commit -m "$1"
    git push
}

readpush(){
    move_rubbish
    git add README.md
    git commit -m "$1"
    git push
}

ispush(){
    move_rubbish
    git add input.S solution
    git commit -m "new input and solution"
    git push
}