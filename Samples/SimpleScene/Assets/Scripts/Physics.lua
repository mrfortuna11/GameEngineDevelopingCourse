local ecs = require "ecs"

local function rand_flt(from, to)
	local maxNumber = 32767
    return from + (math.random(maxNumber) / maxNumber) * (to - from)
end

local function move(it)
    for pos, vel, ent in ecs.each(it) do
        pos.x = pos.x + vel.x * it.delta_time
        pos.y = pos.y + vel.y * it.delta_time
		pos.z = pos.z + vel.z * it.delta_time
    end
end

local function gravity(it)
    for pos, vel, grav, plane, ent in ecs.each(it) do
        local planeEpsilon = 0.1
		
		if plane.x * pos.x + plane.y * pos.y + plane.z * pos.z < plane.w + planeEpsilon then
			do return end
		end
		
		vel.x = vel.x + grav.x * it.delta_time
        vel.y = vel.y + grav.y * it.delta_time
		vel.z = vel.z + grav.z * it.delta_time
    end
end

local function FrictionSystem(it)
    for vel, friction, ent in ecs.each(it) do
        vel.x = vel.x - vel.x * friction.value * it.delta_time
		vel.y = vel.y - vel.y * friction.value * it.delta_time
		vel.z = vel.z - vel.z * friction.value * it.delta_time
    end
end

local function ShiverSystem(it)
    for pos, shiver, ent in ecs.each(it) do
        pos.x = pos.x + rand_flt(-shiver.value, shiver.value)
		pos.y = pos.y + rand_flt(-shiver.value, shiver.value)
		pos.z = pos.z + rand_flt(-shiver.value, shiver.value)
    end
end

local function BounceSystem(it)
    for pos, vel, plane, bounciness, ent in ecs.each(it) do
        local dotPos = plane.x * pos.x + plane.y * pos.y + plane.z * pos.z
		local dotVel = plane.x * vel.x + plane.y * vel.y + plane.z * vel.z
		
		if dotPos < plane.w then
			pos.x = pos.x - (dotPos - plane.w) * plane.x
			pos.y = pos.y - (dotPos - plane.w) * plane.y
			pos.z = pos.z - (dotPos - plane.w) * plane.z

			vel.x = vel.x - (1.0 + bounciness.value) * plane.x * dotVel
			vel.y = vel.y - (1.0 + bounciness.value) * plane.y * dotVel
			vel.z = vel.z - (1.0 + bounciness.value) * plane.z * dotVel
		end
    end
end

local function TimeToKill(it)
    for ttl, ent in ecs.each(it) do
        ttl.ttl = ttl.ttl-1
        if ttl.ttl<=0 then
            ecs.delete(ent)
        end
    end
end

local position = {}
local velocity = {}
local entity = {}
local hit = {}

local function Saving(it)
    for pos, vel, tag, ent in ecs.each(it) do
        table.insert(position, pos)
        table.insert(velocity, vel)
        table.insert(entity, ent)
        table.insert(hit, false)
    end
end

local function Collision(it)
    for pos, vel, tag, ent in ecs.each(it) do
        for i = 1, #position do
            local sq_dist = (position[i].x - pos.x)^2+(position[i].y - pos.y)^2+(position[i].z - pos.z)^2
            if sq_dist<3 then
                tag.setColor = true
                vel.x=vel.x + rand_flt(-10, 10)
                vel.y=vel.y + rand_flt(-10, 10)
                vel.z=vel.z + rand_flt(-10, 10)
                hit[i] = true
            end
        end
    end
end

local function Clear(it)
    for i = 1, #position do
        if hit[i] then
            ecs.delete(entity[i])
        end
    end
    position = {}
    velocity = {}
    entity = {}
    hit = {}
end


ecs.system(move, "Move", ecs.OnUpdate, "Position, Velocity")
ecs.system(gravity, "grav", ecs.OnUpdate, "Position, Velocity, Gravity, BouncePlane")
ecs.system(FrictionSystem, "FrictionSystem", ecs.OnUpdate, "Velocity, FrictionAmount")
ecs.system(ShiverSystem, "ShiverSystem", ecs.OnUpdate, "Position, ShiverAmount")
ecs.system(BounceSystem, "BounceSystem", ecs.OnUpdate, "Position, Velocity, BouncePlane, Bounciness")
ecs.system(TimeToKill, "TimeToKill", ecs.OnUpdate, "BulletKill")
ecs.system(Saving, "Saving", ecs.OnUpdate, "Position, Velocity, BulletKill")
ecs.system(Collision, "Collision", ecs.OnUpdate, "Position, Velocity, Object")
ecs.system(Clear, "Clear", ecs.OnUpdate, "Player")