-- Shared TexAreas...
-- Note that directions mean "not me in that direction",
-- so a UL means "I am not to my Up or Left, but I am to the left, and to the right".
local r = 1.0 / 4  -- 4x4 TexArea ratio.

-- Order key: U, D, L, R
local area_ul = TexArea( r * 0, r * 0, r * 1, r * 1 )
local area_u = TexArea( r * 1, r * 0, r * 2, r * 1 )
local area_ur = TexArea( r * 2, r * 0, r * 3, r * 1 )
local area_l = TexArea( r * 0, r * 1, r * 1, r * 2 )
local area_center = TexArea( r * 1, r * 1 , r * 2, r * 2 )
local area_r = TexArea( r * 2, r * 1, r * 3, r * 2 )
local area_dl = TexArea( r * 0, r * 2, r * 1, r * 3 )
local area_d = TexArea( r * 1, r * 2, r * 2, r * 3 )
local area_dr = TexArea( r * 2, r * 2, r * 3, r * 3 )

local area_udl = TexArea( r * 0, r * 3, r * 1, r * 4 )
local area_udr = TexArea( r * 2, r * 3, r * 3, r * 4 )
local area_ulr = TexArea( r * 3, r * 0, r * 4, r * 1 )
local area_dlr = TexArea( r * 3, r * 2, r * 4, r * 3 )

local area_ud = TexArea( r * 1, r * 3, r * 2, r * 4 )
local area_lr = TexArea( r * 3, r * 1, r * 4, r * 2 )


local area_surround = TexArea( r * 3, r * 3, r * 4, r * 4 )


function CreateInvalid( terra_size )
	local parameters = TerraParameters()	
	parameters:SetSize( terra_size )
	parameters:SetConstant( 0 )	
	parameters:SetPoints( 2, 2 )	-- In this case, vertices... minimum of 2x2 (quad)
	

	local vs = VertexShader( "texture" )
	local ps = PixelShader( "texture" )
	local t1 = Texture( "invalid" )
	parameters:SetEffect( Effect( vs, ps, t1 ) )

	parameters:SetTexArea( TexArea() )
	Terra( "invalid", parameters )
end

function CreateGrass( terra_size )
	local parameters = TerraParameters()	
	parameters:SetSize( terra_size )
	parameters:SetConstant( 0 )	
	parameters:SetPoints( 2, 2 )	-- In this case, vertices... minimum of 2x2 (quad)
	
	local vs = VertexShader( "texture" )
	local ps = PixelShader( "texture" )
	local t1 = Texture( "grass" )
	parameters:SetEffect( Effect( vs, ps, t1 ) )

	parameters:SetTexArea( TexArea() )
	Terra( "grass", parameters )
end

-- Simplifies making the terra, when it follows the "mix" paradigm...
function MakeMixTerra( terra_size, name, parameters )
	parameters:SetTexArea( area_ul )
	Terra( name .. "_ul", parameters )

	parameters:SetTexArea( area_u )
	Terra( name .. "_u", parameters )

	parameters:SetTexArea( area_ur )
	Terra( name .. "_ur", parameters )

	parameters:SetTexArea( area_l )
	Terra( name .. "_l", parameters )

	parameters:SetTexArea( area_center )
	Terra( name .. "_center", parameters )

	parameters:SetTexArea( area_r )
	Terra( name .. "_r", parameters )

	parameters:SetTexArea( area_dl )
	Terra( name .. "_dl", parameters )

	parameters:SetTexArea( area_d )
	Terra( name .. "_d", parameters )

	parameters:SetTexArea( area_dr )
	Terra( name .. "_dr", parameters )

	
	-- 3s...
	
	parameters:SetTexArea( area_udl )
	Terra( name .. "_udl", parameters )

	parameters:SetTexArea( area_udr )
	Terra( name .. "_udr", parameters )

	parameters:SetTexArea( area_ulr )
	Terra( name .. "_ulr", parameters )

	parameters:SetTexArea( area_dlr )
	Terra( name .. "_dlr", parameters )
	
	
	-- 2s...
	
	parameters:SetTexArea( area_ud )
	Terra( name .. "_ud", parameters )

	parameters:SetTexArea( area_lr )
	Terra( name .. "_lr", parameters )

	
	-- Surround...

	parameters:SetTexArea( area_surround )
	Terra( name .. "_surround", parameters )
	
	Scene():SendCommand( "BuildMapGround", name )
end

function CreateSand( terra_size )
	local parameters = TerraParameters()	
	parameters:SetSize( terra_size )
	parameters:SetConstant( 0 )	
	parameters:SetPoints( 2, 2 )
	
	local vs = VertexShader( "texturemix_ambient")
	local ps = PixelShader( "texturemix_ambient" )
	local t1 = Texture( "sand" )
	local t2 = Texture( "grass" )
	local t3 = Texture( "sand_mix" )
	local effect = Effect( vs, ps, t1, t2, t3 )
	parameters:SetEffect( effect )
	
	MakeMixTerra( terra_size, "sand", parameters )
end

function CreateSidewalk( terra_size )
	local parameters = TerraParameters()	
	parameters:SetSize( terra_size )
	parameters:SetConstant( 0 )	
	parameters:SetPoints( 2, 2 )
	
	local vs = VertexShader( "texturemix_ambient")
	local ps = PixelShader( "texturemix_ambient" )
	local t1 = Texture( "wall" )
	local t2 = Texture( "leafsolid" )
	local t3 = Texture( "sidewalk_mix" )
	local effect = Effect( vs, ps, t1, t2, t3 )
	parameters:SetEffect( effect )
	
	MakeMixTerra( terra_size, "sidewalk", parameters )
end

function CreateTerrain( map_size, terra_size )
	local this = Scene()
	this:SendCommand( "SetMapWidth", tostring(map_size:width()) )
	this:SendCommand( "SetMapHeight", tostring(map_size:height()) )
	this:SendCommand( "SetMapTerraWidth", tostring(terra_size:width()) )
	this:SendCommand( "SetMapTerraHeight", tostring(terra_size:height()) )
	this:SendCommand( "MakeMap" )
	CreateInvalid( terra_size )
	CreateGrass( terra_size )
	CreateSand( terra_size )
	CreateSidewalk( terra_size )
	
	this:SendCommand( "AddMapFriend", "sand, ".. "sidewalk" )
end