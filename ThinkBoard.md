# Think board

Ideas

        -----   Render object ----
	      /                         \
	    Model --------------        Position/Rotation/Scale in world space
	     /                  \
       |                  Positions/Rotations/Scales of each part in model space
      MeshParts ------
      /                \
   Material             Mesh (sets of vertices in mesh space and allocated buffer)
   /       \
  /        Sets (encore flou pour cet aspect)
 /
 ------ShaderEffects------------
   /            \               \
ShaderModule  Pipeline & layout  Sets layout



V2                                                                  Renderer ---- other things
                                                                ////
                      --------------------------------------- RenderNode ----------------------------
                    /                                    /                     /                     \
                   /                                    /                     /                       \
                 --- Model -------------------           parent (node id)     children (node id)       Transform
                 /                             \                                                 (rel. parent ?world space ?)
                ---- Material ------------      Mesh (vertices + faces)
               /                          \
      MaterialTemplate                    Descriptor Sets (et autres trucs)
      |||
      |||
   ShaderEffects
 (shader modules,
 pipelines,
 set layouts...)

=== FOR EACH SHADER KIND, DRAW THE OBJECTS OF WHICH THE MATERIAL HAS A TEMPLATE THAT CONTAINS A SHADER EFFECT OF THAT KIND ===


bind global descriptor set
bind vertex buffer with all meshes

FOR EACH SHADER KIND:
    TAKE THE OBJECTS WITH A MATERIAL WITH A SHADER EFFECT OF THAT KIND

    FOR EACH DIFFERENT SHADER EFFECT
      bind pipeline
-
      FOR EACH DIFFERENT MATERIAL
        bind material descriptor set

        FOR EACH MODEL WITH THAT MATERIAL
          draw instances




***

Node id: 1 ou +. 0 = null (ex: root node has no parent)


=== REQUIREMENTS OF THE ARCHITECTURE ===

- Nodes/parts should be structured as a TREE
- Shader effect = pipeline + everything needed to build it.
    -> Objective: as little effects as possible (limit the amount of pipelines we need to bind).
- Material Template = collections of available effects (ex one for shadows, one for forward textured...). Other things later.
- Material = Created from a Template. Also contains descriptor sets for the textures.
- Mesh = set of vertices and index. They are all stored in a single, big vertex buffer bound only once.
- Model = unique mesh/material configurations, associates a mesh id with a material id.
- Nodes = objects themselves. they have a tree architecutre, a transform. They refer to models. Several nodes can refer to the same model, in which case instancing will occur.


Vertex / index buffer -> bound once

Pipeline -> once per unique pipeline, but most material merge through templates and thus can be grouped together.

Descriptor Sets
  -> global (camera data, scene data...)
  -> material (textures + other parameters)


SUMMARY

begin: bin global set + vertex and index buffers

for each material template:
  bind the pipeline
  for each material:
    bind the descriptor set with the texture



BINDLESS
--------

-> Meshes: All meshes in 1 buffer + "baseVertex" offset in the draw call
-> Textures: Texture arrays + texture index from other buffer
-> Materials: not 1 pipeline per material + material params in ssbo





-------------------------
PLAN

1. UTILISER DES STRUCTS POUR LES ALLOCATIONS
2. resize if needed
3. register commands
4. draw



what we need to draw indirect ?

-> a bunch of batches

batch:
  -> start
  -> size (nb of commands in a single draw indirect = 1 per model in the material)
  -> 