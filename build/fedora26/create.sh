#!/bin/bash

docker \
  image \
  build \
  --tag qfm1000/fedora26 \
  .

docker \
  container \
  create \
  --name qfm1000_fedora26 \
  qfm1000/fedora26

echo "To use it, runs     docker run --mount type=bind,source=$(pwd)/../../,target=/qfm1000 -it qfm1000/fedora26 /bin/bash"
