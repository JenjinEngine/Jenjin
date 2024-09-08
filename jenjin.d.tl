global record vec2
	x: number
	y: number

	new:function(self:vec2, x:number, y:number): vec2
end

global record vec3
	x: number
	y: number
	z: number

	new:function(self:vec3, x:number, y:number, z:number): vec3
end

global record Transform
	position: vec2
	rotation: number
	scale: vec2
	z_index: number

	new:function(self:Transform, position:vec2, rotation:number, scale:number, z_index:number): Transform
end

global record GameObject
	id: number
	name: string
	transform: Transform
	mesh_id: number
	color: vec3

	new:function(self:GameObject, mesh_data:MeshData, name:string): GameObject

	set_transform:function(self:GameObject, transform:Transform)
	translate:function(self:GameObject, translation:vec2)
	set_position:function(self:GameObject, position:vec2)
	set_rotation:function(self:GameObject, rotation:number)
	rotate:function(self:GameObject, rotation:number)
	fill_in_id:function(self:GameObject)
end

global record Scene
	get_gameobject:function(self:Scene, name:string): GameObject
	add_gameobject:function(self:Scene, gameobject:GameObject)
	build:function(self:Scene)
end

global record state
	dt: number
	window: table
	engine: table
	scene: Scene
end

global record s
	dt: number
	window: table
	engine: table
	scene: Scene
end

global record MeshData
	vertices: table
	indices: table
end

global record Mesh
	base_vertex: number
	base_index: number
	vertex_count: number
	index_count: number
end

global GLFW_KEY_W: number
global GLFW_KEY_A: number
global GLFW_KEY_S: number
global GLFW_KEY_D: number
global GLFW_KEY_SPACE: number

global is_key_pressed: function(key: number): boolean
global create_quad: function(width: number, height: number): MeshData
global nilgobj: function(): GameObject

global function print(_:string) end
global function info(_:string) end
global function warn(_:string) end
global function error(_:string) end
global function debug(_:string) end
global function trace(_:string) end

global function normalize(v:vec2): vec2 end
global function normalize_v2(v:vec2): vec2 end
global function normalize_v3(v:vec3): vec3 end
