# PDI

- implementar where is X com correlação?
- implementar filtro de borramento usando convolução?
- implementar light paint.

Notes:

- Resize images:

```sh
sips -Z 640 *.jpg
# Or preserve the originals
mkdir -p resized && sips -Z 640 *.jpg --out resized/
```