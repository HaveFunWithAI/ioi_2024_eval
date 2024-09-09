#!/bin/bash
cd message
ulimit -v 2097152
ulimit -s 2097152
timeout 5s ./message < ./tests/1-04.in > ./claude-3-5-sonnet/1-04.out
timeout 5s ./message < ./tests/0-01.in > ./claude-3-5-sonnet/0-01.out
timeout 5s ./message < ./tests/2-20.in > ./claude-3-5-sonnet/2-20.out
timeout 5s ./message < ./tests/1-02.in > ./claude-3-5-sonnet/1-02.out
timeout 5s ./message < ./tests/2-18.in > ./claude-3-5-sonnet/2-18.out
timeout 5s ./message < ./tests/2-08.in > ./claude-3-5-sonnet/2-08.out
timeout 5s ./message < ./tests/1-05.in > ./claude-3-5-sonnet/1-05.out
timeout 5s ./message < ./tests/1-01.in > ./claude-3-5-sonnet/1-01.out
timeout 5s ./message < ./tests/2-10.in > ./claude-3-5-sonnet/2-10.out
timeout 5s ./message < ./tests/2-05.in > ./claude-3-5-sonnet/2-05.out
timeout 5s ./message < ./tests/2-19.in > ./claude-3-5-sonnet/2-19.out
timeout 5s ./message < ./tests/2-09.in > ./claude-3-5-sonnet/2-09.out
timeout 5s ./message < ./tests/2-14.in > ./claude-3-5-sonnet/2-14.out
timeout 5s ./message < ./tests/2-11.in > ./claude-3-5-sonnet/2-11.out
timeout 5s ./message < ./tests/2-04.in > ./claude-3-5-sonnet/2-04.out
timeout 5s ./message < ./tests/2-02.in > ./claude-3-5-sonnet/2-02.out
timeout 5s ./message < ./tests/2-12.in > ./claude-3-5-sonnet/2-12.out
timeout 5s ./message < ./tests/2-17.in > ./claude-3-5-sonnet/2-17.out
timeout 5s ./message < ./tests/2-03.in > ./claude-3-5-sonnet/2-03.out
timeout 5s ./message < ./tests/1-07.in > ./claude-3-5-sonnet/1-07.out
timeout 5s ./message < ./tests/2-13.in > ./claude-3-5-sonnet/2-13.out
timeout 5s ./message < ./tests/2-06.in > ./claude-3-5-sonnet/2-06.out
timeout 5s ./message < ./tests/1-06.in > ./claude-3-5-sonnet/1-06.out
timeout 5s ./message < ./tests/1-03.in > ./claude-3-5-sonnet/1-03.out
timeout 5s ./message < ./tests/2-07.in > ./claude-3-5-sonnet/2-07.out
timeout 5s ./message < ./tests/2-16.in > ./claude-3-5-sonnet/2-16.out
timeout 5s ./message < ./tests/2-15.in > ./claude-3-5-sonnet/2-15.out
timeout 5s ./message < ./tests/2-01.in > ./claude-3-5-sonnet/2-01.out
