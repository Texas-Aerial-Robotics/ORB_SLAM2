FILES=src/*
for f in $FILES
do
  sed -i -e 's/iORB_SLAM/ORB_SLAM2/g' "$f"
done
