local ecs = require "ecs"

local function timeUpdate(it)
    for t, ent in ecs.each(it) do
        t.time = t.time + it.delta_time
    end
end

local function destroyObject(it)
    for pos, t, dest, ent in ecs.each(it) do
        if t.time > dest.toDestroy then
            pos.y = 10000.0
        end
    end
end

local function collisionDetection(it)
    for posA, collisionA, entA in ecs.each(it) do
        for posB, collisionB, entB in ecs.each(it) do
            if entA ~= entB then
                if 	(posA.x - posB.x) * (posA.x - posB.x) + 
					(posA.y - posB.y) * (posA.y - posB.y) + 
					(posA.z - posB.z) * (posA.z - posB.z) < 3.0 then
                    collisionA.intersect = true
                    collisionB.intersect = true
                    if collisionA.shot then
                        collisionB.bullet = true
                    end

                    if collisionB.shot then
                        collisionA.bullet = true
                    end					
                else
                    collisionA.intersect = false
                    collisionB.intersect = false
                end
            end
        end
    end
end

local function shotCheck(it)
    for pos, collision, ent in ecs.each(it) do
        if collision.bullet then
            pos.y = pos.y + 4.0
        end
    end
end

local function bounceAfterCollision(it)
    for pos, vel, collision, ent in ecs.each(it) do
        if collision.intersect then
            vel.z = vel.z * -1.0
            pos.z = pos.z + 1.5 * vel.z
            collision.intersect = false
        end
    end
end

ecs.system(timeUpdate, "timeUpdate", ecs.OnUpdate, "Timer")
ecs.system(destroyObject, "DestroyObject", ecs.OnUpdate, "Position, Timer, ToDestroy")
ecs.system(collisionDetection, "CollisionDetection", ecs.OnUpdate, "Position, Collider")
ecs.system(shotCheck, "ShotCheck", ecs.OnUpdate, "Position, Collider")
ecs.system(bounceAfterCollision, "BounceAfterCollision", ecs.OnUpdate, "Position, Velocity, Collider")