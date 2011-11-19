----
-- Eduardo Ceretta Dalla Favera
-- eduardo.ceretta@gmail.com
-- Nov 2011
-- 
----

function PrintTable( t, indentParam )
  if not t then
    return
  end
  local indent = indentParam or " "
  local i, v 
  for i,v in pairs(t) do
    if type( v ) == "table" then
      print( indent .. tostring( i ) .." = {"  )
      PrintTable( t[i] , indent .. " ")
      print( indent .. "  }"  )
    else
      print( indent .. tostring( i ) .." = " .. tostring( v ) )
    end
  end
end

function unpatternify(str)
  local gsubUnpatternTable = {
    ["^"] = "%^",
    ["$"] = "%$",
    ["("] = "%(",
    [")"] = "%)",
    ["%"] = "%%",
    ["."] = "%.",
    ["["] = "%[",
    ["]"] = "%]",
    ["*"] = "%*",
    ["+"] = "%+",
    ["-"] = "%-",
    ["?"] = "%?"
  }
  
  return str:gsub("[%^%$%(%)%.%[%]%*%+%-%?%%]", gsubUnpatternTable)
end

--Needs ImageMagick-6.7.3-6  with files renamed to IM. available at the path
function generateImageDiff(file1, file2, diffFile)
  os.execute("IM.convert " .. file1 .. " " .. file2 .. " -compose difference -composite -evaluate Pow 2 -evaluate divide 3 -separate -evaluate-sequence Add -evaluate Pow 0.5 " .. diffFile)
end

--Needs ImageMagick-6.7.3-6  with files renamed to IM. available at the path
function calcMetric(file1, file2, metric)
  metric = metric and metric or "PSNR"
  local tmpFile = os.tmpname()
  local tmpFile2 = os.tmpname()

  os.execute("IM.compare -metric " .. metric .. " " .. file1 .. " " .. file2 .. " " .. tmpFile .. " 2> " .. tmpFile2)
  
  retMetric = io.lines(tmpFile2)()
 
  -- os.execute("del " .. tmpFile)
  -- os.execute("del " .. tmpFile2)
  return retMetric, metric
 end


--opt = {
  -- recursive = true,
  -- filter = "", example "*.txt"
-- }
function dirRecursive(opt)
  opt = opt or {filter = "", recursive = false}
  opt.filter = opt.filter and opt.filter or ""
  opt.recursive = opt.recursive and "/s" or ""
  
  local dirFile = ""..os.tmpname()
  os.execute("cd > " .. dirFile)
  os.execute("dir /b ".. opt.recursive .. " " .. opt.filter .. " >> " .. dirFile)
  
  local fileItr = io.lines(dirFile)
  local localPath = fileItr().."\\"
  local retTable = {}
  
  for line in fileItr do
    line = line:gsub(unpatternify(localPath), "")
    local last = line:match(".*\\([^\\/:%?%*<>]+)")

    if last ~= nil then
      local fileName = line:gsub(unpatternify(last), "")
      local t = retTable
      for w in fileName:gmatch("([^\\/:%?%*<>]+)\\") do 
        if t[w] == nil or type(t[w]) == type("") then
          t[w] = {}
        end
        t = t[w]
      end
      t[last] = line
    else 
      last = line
      retTable[last] = last
    end
  end
  
  os.execute("del " .. dirFile)
  return retTable, localPath
end

rt, path = dirRecursive{filter = "*.png", recursive = false}
-- PrintTable(rt)

local poseTable = {
  ["top"]    = {referenceFileName = "reference\\_top.png"   , diffOutputPath = "Diff\\top"   , bestImg = ""},
  ["botton"] = {referenceFileName = "reference\\_botton.png", diffOutputPath = "Diff\\botton", bestImg = ""},
  ["right"]  = {referenceFileName = "reference\\_right.png" , diffOutputPath = "Diff\\right" , bestImg = ""},
  ["left"]   = {referenceFileName = "reference\\_left.png"  , diffOutputPath = "Diff\\left"  , bestImg = ""},
  ["front"]  = {referenceFileName = "reference\\_front.png" , diffOutputPath = "Diff\\front" , bestImg = ""},
  ["back"]   = {referenceFileName = "reference\\_back.png"  , diffOutputPath = "Diff\\back"  , bestImg = ""},
}

for i, j in pairs(rt) do
  local algorithm, pose, attribs = i:match("screen_%d+_%d+_%d+-%d+_%d+_%d+-%d+_(.*)-(.*)-(.*)")
  if algorithm then
    if poseTable[pose] ~= nil then
      table.insert(poseTable[pose], {alg = algorithm, attrib = attribs,  file = j})
    end
  end
end

for poseName, poseItemTable in pairs(poseTable) do
  print(poseName)
  local maxMetric = -9999999999999999.99
  for i, imgTable in ipairs(poseItemTable) do
    local metric, metricName = calcMetric(poseItemTable.referenceFileName, imgTable.file)
    local fileName = poseItemTable.diffOutputPath.."\\Diff_" .. metricName .. "=" .. metric .. "-" .. imgTable.alg.. "-" .. imgTable.attrib
    generateImageDiff(poseItemTable.referenceFileName, imgTable.file, fileName)
    if tonumber(metric) > maxMetric then
      maxMetric = tonumber(metric)
      poseItemTable.bestImg = imgTable.file
    end
    print("  Comparing file ".. imgTable.file)
    print("    Generated file ".. fileName)
    print("    "..metricName.." Metric: " .. metric)
    print("    Attributes: " .. imgTable.attrib)
    print""
  end
end

print""
print"Best Images:"
for poseName, poseItemTable in pairs(poseTable) do
  print(" " .. poseName, poseItemTable.bestImg)
end

--[[
----
----
function executeProgram(testpath, logdir, appname, args)
	os.execute('@ECHO Executing '..appname .. ' with args: '..args)
	os.execute(testpath .. appname..' '..args)
  os.execute('@ECHO Moving test results to '..logdir)
  os.execute('mkdir ' .. logdir)
  os.execute('move *.png ' .. logdir) 
  os.execute('move *.log ' .. logdir) 
	os.execute('@ECHO End execution')
end

local testpath = "TssaoRelease\\"
local shaderpath = "Resources/Shaders/"
local scenepath =  "Resources/Scenes/scene1.rt4"
local args = " -shaderpath " .. shaderpath .. " -scenepath " .. scenepath
local logdir = "Ztests\\"
local appname = "tssao.exe"

local alg_args = ""
local rfar = 0
local intensity = .5
local offset = 1
for rfar = 1, 10, 4 do
  local rfar_arg = " -rfar " .. rfar
  for offset = 1, 6, 2 do
    local offset_arg = " -offset " ..  offset
    for intensity = 1, 31, 10 do
      local intensity_arg = " -intensity " .. intensity
      local logdir_spec = logdir .. "r_" .. rfar ..  "_o_" .. offset .. "_i_" .. intensity .. "\\"
      executeProgram(testpath, logdir_spec, appname, args .. rfar_arg .. intensity_arg .. offset_arg)
    end
  end
end
]]
