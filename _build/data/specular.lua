function math.clamp(v, a, b)
  return math.min(math.max(v, a), b)
end

-- make screen resizable
screen.set(800, 600, false, true)

-- setup camera
camera.setclearcolor(0, _COLOR_DARKGRAY)
camera.setposition(0, 0, 0, -6)
camera.setrotation(0, 0, 0, 0)

-- setup lighting
light.setrotation(0, 30, 20)

-- load sphere
object.load(0, "sphere.msh")
object.setcolor(0, 0, _COLOR_ORANGE)
--object.setshininess(0, 0, 0)

while screen.opened() and not input.keydown(_KEY_ESC) do
  -- update shininess
  if input.keydown(_KEY_LEFT) then object.setshininess(0, 0, math.clamp(object.shininess(0, 0) - 0.2 * screen.delta(), 0, 1)) end
  if input.keydown(_KEY_RIGHT) then object.setshininess(0, 0, math.clamp(object.shininess(0, 0) + 0.2 * screen.delta(), 0, 1)) end

  -- print
  screen.clearprints()
  screen.print(math.floor(screen.fps()) .. " FPS")
  screen.print("Shininess: " .. object.shininess(0, 0) .. " (LEFT and RIGHT to change)")
  screen.sync()
end
