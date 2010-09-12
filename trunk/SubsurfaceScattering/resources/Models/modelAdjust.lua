local fileName = "sphere.msh"

local file = io.open(fileName, "r")
if file == nil then
  print("Could not find file " ..  fileName)
  io.read()
  return
end

local line = file:read()
-- local line = "588 960"
local i, i2, n1, n2 = string.find(line,"(%d+)%s(%d+)")

local vertexTable = {}

for i=1, n1 do
  line = file:read()
  -- line = "0 -0.232019 -24.201216 -0.464038"
  local i, i2, index, x, y, z = string.find(line,"(%d+)%s(-*%d+.%d+)%s(-*%d+.%d+)%s(-*%d+.%d+)")
  local t = {""..x..y..z, x, y, z}
  vertexTable[t[1]] = vertexTable[t[1]] and vertexTable[t[1]] + 1 or 1
end

for i, j  in pairs(vertexTable) do
  if j > 1 then
    print(i, j)
  end
end

file:close()
io.read()
