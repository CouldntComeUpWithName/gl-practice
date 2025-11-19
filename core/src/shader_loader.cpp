#include <core/shader_loader.h>
#include <core/shader.h>

gfx::shader Loader<gfx::shader>::load(const load_context& ctx) 
{
  gfx::shader shader;
  
  if(ctx.paths.size() == 2) 
  {
    shader.load(ctx.paths[0], ctx.paths[1]);
  }
  else if(ctx.paths.size() == 3)
  {
    shader.load(ctx.paths[0], ctx.paths[1], ctx.paths[2]);
  }
  
  return shader;
}