const dmc = require('./');

const voxels = dmc.getTestTriangles();

voxels.triangles.map(triangle => {
  console.log('Triangle on ', triangle[0], triangle[1], triangle[2]);
});
