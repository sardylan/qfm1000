#!/bin/bash

docker \
  image \
  build \
  --tag qfm1000/ubuntu2004 \
  .

docker \
  container \
  create \
  --name qfm1000_ubuntu2004 \
  qfm1000/ubuntu2004

echo "To use it, runs     docker run --mount type=bind,source=$(pwd)/../../,target=/qfm1000 -it qfm1000/ubuntu2004 /bin/bash"
