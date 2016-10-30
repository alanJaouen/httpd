#!/bin/sh

if [ $# -ne 2 ]; then
  echo "Usage: $0 ip port"
  exit 1
fi

wget $1:$2/README -o /tmp/t
cat /tmp/t | grep "200" > /tmp/tt
if [ "$(cat /tmp/tt)" = "" ]; then
  echo GET on existing file KO
else
  echo GET on existing file OK
fi

wget $1:$2/README23 -o /tmp/t
cat /tmp/t | grep "404" > /tmp/tt
if [ "$(cat /tmp/tt)" = "" ]; then
  echo GET on non existing file KO
else
  echo GET on non existing file OK
fi


wget $1:$2/src -o /tmp/t
cat /tmp/t | grep "404" > /tmp/tt
if [ "$(cat /tmp/tt)" = "" ]; then
  echo GET on dir KO
else
  echo GET on dir OK
fi

touch forbidden
chmod 000 forbidden

wget $1:$2/tests/forbidden -o /tmp/t
cat /tmp/t | grep "403" > /tmp/tt
if [ "$(cat /tmp/tt)" = "" ]; then
  echo GET on forbidden file KO
else
  echo GET on forbidden file OK
fi
rm forbidden
rm README
