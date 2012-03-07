-----------------------------------------------------------------------------------
-- Eduardo Ceretta Dalla Favera
-- eduardo.ceretta@gmail.com
-- 26 Feb 2012
-- 
-- Test the aplication. Varies the program input trough the command line and
-- scene file. Performs a series of tests anda generate a log file containing
-- the result of each test and the respectives screenshots.
-----------------------------------------------------------------------------------
executionEnabled = true

workingDir = "../../" -- from here

--DEBUG
-- programPath = "prj/ssao/vc9/testeSSAO_Debug/" -- from working dir
--RELEASE
programPath = "prj/ssao/vc9/testeSSAO_Release/" -- from working dir
programName = "testeSSAO.exe"

--Program Needed Paths
scenePath = "resources/Tests/testeScene.rt4"
shaderPath = "resources/Shaders/"

--Test Paths
logFilePath = "resources/Tests/Logs/"
-- logFileName = ""
screenShotsPath = "resources/Tests/ScreenShots/MainTest/"

--Enabled Tests
activeTests = {
  -- algorithmCompare = true,
  -- parameterVariation = true,
  -- scalability = true,
  screenshotQuality = true,
  -- timeTable = true,
}

--Program Aceptable Parameters
parameters = {
  scenepath = "-scenepath",
  shaderpath = "-shaderpath",
  
  logPath = "-logPath",
  logFileName = "-logFileName",
  screenShotsPath = "-screenShotsPath",

  screenShotTestEnabled  = "-test:screenshot",
  timeTestEnabled = "-test:time",

  sphereEnabled = "-alg:Sphere",
  rayMarchEnabled = "-alg:RayMarch",
  coneTracingEnabled = "-alg:ConeTracing",

  sphere_rfar = "-parm:Sphere:rfar",
  sphere_offset = "-parm:Sphere:offset",
  sphere_contrast = "-parm:Sphere:contrast",

  rayMarch_rfar = "-parm:RayMarch:rfar",
  rayMarch_contrast = "-parm:RayMarch:contrast",

  coneTracing_jitter = "-parm:ConeTracing:jitter",
  coneTracing_rfar = "-parm:ConeTracing:rfar",
  coneTracing_contrast = "-parm:ConeTracing:contrast",
  coneTracing_coneAngle = "-parm:ConeTracing:coneAngle",
  coneTracing_numCones = "-parm:ConeTracing:numCones",
  coneTracing_numSpheres = "-parm:ConeTracing:numSpheres",
  coneTracing_numSamplers = "-parm:ConeTracing:numSamplers",
}

-- Models Used in Tests. Sorted by importance. Not all will be used.
testModels = {
  path = "resources/Models/obj/Watertight/",
  {--1
    name = "cutter.msb",
    poses = {
      "4.1874 7.09915 151.792         4.1874 7.09915 -0.7609               0 1 0                           30       5      1000",
      "-99.3127 -14.3746 -33.4869     2.77799 8.8365 -1.57661              -0.127869 0.950722 -0.28245     30       5      1000",
      "105.553 23.2566 -36.3459       2.77799 8.8365 -1.57661              -0.212298 0.948763 -0.234048    30       5      1000",
    },
  },
  {--2
    name = "city.msb",
    poses = {
      "0.615725 0.236336 2.21997      0.615725 0.236336 -0.579334          0 1 0                            30       5      1000",
      "-2.41815 1.60309 0.720007      0.615725 0.236336 -0.579334          0.351855 0.923911 -0.150287      30       5      1000",
      "1.27591 0.72413 -2.41955       0.615725 0.236336 -0.579334          0.0543153 0.960192 0.274009      30       5      1000",
    },
  },
  {--3
    name = "DW-Ormesh-02.msb",
    poses = {
      "26.4077 180.487 1009.17        26.4077 180.487 -178.905             0 1 0                           30       5      1000",
    },
  },
  {--4
    name = "dragon.msb",
    poses = {
      "-28.1103  5.09934  -2.07215      -6.61092e-008  4.96995  -4.15856e-009    0.00135533  0.999031  0.043997     30    5     1000",
      -- "27.7427  1.57225  -3.64703       3.3931e-007  4.96995  7.40699e-007       0.110213  0.990331  -0.084241      30    5     1000",
      "0.699758  6.52407  28.1354       8.94602e-007  4.96995  8.67915e-007      0.00236674  0.998472  -0.0552121   30    5     1000",
      -- "0.514386  2.11135  -28.0369      8.53611e-007  4.96995  8.28582e-007      0.00899126 0.994819  -0.101265     30    5     1000",
      "0.434503  32.6008  -5.55447      8.12403e-007  4.96995  7.57458e-007      -0.0748029 0.197659  0.977413      30    5     1000",
      -- "3.24169  -23.0011  1.27389       8.93577e-007  4.96995  1.06895e-006      0.0368047  -0.041208  -0.998473    30    5     1000",
    },
  },
  {--5
    name = "ateneam.msb",
    poses = {
      "9395.73  1031.21  6815.89      -1311.34  873.505  25.674      -0.0161194  0.999868  0.00219707  30  5  1000",
      "2012.83  163.739  -1269.32      -1148.92  1750.66  316.511      0.368892  0.912364  -0.177512  30  5  1000",
    },
  },
  {--6
    name = "ducky.msb",
    poses = {
      "423.386  154.749  440.803      6.99708e-005  52.8394  -41.1333      -0.0947344  0.987548  -0.1256  30  5  1000",
      "-77.3621  148.57  190.565      45.8546  101.377  -32.59      0.031042  0.981222  -0.190369  30  5  1000",
    },
  },
  {--7
    name = "venusm.msb",
    poses = {
      "0.62915  1434.35  8945.67      0.62915  1434.35  -186.891      0  1  0  30  5  1000",
      "555.918  3948.94  944.666      32.0453  2893.19  47.401      -0.930649  0.203139  0.304345  30  5  1000",
    },
  },
  {--8
    name = "elepham.msb",
    poses = {
      "-1642.09  296.865  1036.14      187.186  467.413  110.326      -0.0938503  0.995584  -0.00203468  30  5  1000",
      "357.454  670.061  700.947      12.4391  497.878  226.295      -0.303574  0.944949  -0.122126  30  5  1000",
    },
  },   
  {--9
    name = "ladybird.msb",
    poses = {
      "1261.85  144.762  -508.307      0.42215  186.778  16.8873      0.00439427  0.997589  -0.0692558  30  5  1000",
      "83.9651  -275.241  -256.601      41.5013  -15.0654  -129.425      -0.00770198  0.438123  -0.898882  30  5  1000",
    },
  },    
  {--10
    name = "mba1.msb",
    poses = {
      -- "-9.30467  64.0636  416.061      -9.30467  64.0636  0.0113792      0  1  0  30  5  1000",
      "96.3126  89.7726  141.142      11.4312  68.186  -22.31      -0.0831427  0.992652  -0.0879209  30  5  1000",
    },
  },      
  {--11
    name = "donk.msb",
    poses = {
      "-74.9741  56.2721  54.43      2.5454  26.1191  1.26738      0.326969  0.94324  -0.0582187  30  5  1000",
      -- "-6.49261  23.9314  40.183      8.32765  35.5926  4.02966      0.134818  0.925554  0.3538  30  5  1000",
    },
  },  
  {--12
    name = "cornellbox.msb",
    poses = {
      "2.39071  2.76953  6.49835    2.54724  2.37149  -2.66228    -0.0133787  0.998958  -0.0436348          30       5      1000",
    },
  },    
}


function createSceneFile(sceneName, resolution, models)
  local file = io.open(sceneName, "w")
  
  if file then
    file:write("RT 4.0\n")
    file:write("//File generated automatically!\n")
    --Scene
    file:write("SCENE 0.341 0.549 0.85 0.2 0.2 0.2 " .. tostring(resolution[1]) .. "  " .. resolution[2] .. "\n")
    --Default Camera
    file:write("CAMERA   100 100 40  0 0 0     0 1 0     30    5   1000 \n")
    --Material
    file:write("MATERIAL   .8 .8 .8     1. 1. 1.        50.0           0.0            0.8         0.4 \n")
    --Light
    file:write("LIGHT  .25 .5 .83   .85 .85 .85   .5 .5 .5\n\n")
    --Models
    local meshStr = "MESH 0 0 0 0 1 1 1 " .. models.path
    local poseStr = "CAMERA "
    for i, model in ipairs(models) do
      file:write(meshStr .. model.name .. " \n")
      for j, pose in ipairs(model.poses) do
        file:write(poseStr .. pose .. "\n")
      end
      file:write("\n")
    end
    
    file:close()
    print("SceneFile " .. sceneName .. " Created")
  end
end

function runTestProgram(logFileName, scenePath, activeProgramTests, activeAlgorithms, algParameters)
  local workDirCmd = "cd " .. workingDir .. " & "
  local executablePath = "\"" .. programPath .. programName .. "\" "
  
  local args = parameters.shaderpath .. ' "' .. shaderPath .. '"   ' 
            .. parameters.scenepath .. ' "' .. scenePath .. '"   ' 
            .. parameters.logPath .. ' "' .. logFilePath .. '"   '
            .. parameters.logFileName .. ' "' .. logFileName .. '"   '
            .. parameters.screenShotsPath .. ' "' .. screenShotsPath .. '"   '
  for i, test in ipairs(activeProgramTests) do
    args = args .. test .. "   " 
  end
  
  for i, algorithm in ipairs(activeAlgorithms) do
    args = args .. algorithm .. "   " 
  end 
  
  for i, parms in ipairs(algParameters) do
    args = args .. parms[1] .. " " .. tostring(parms[2]) .. "   " 
  end
  
  local cmdLine = workDirCmd .. executablePath .. args
  print(cmdLine.."\n")
  
  if executionEnabled then
    os.execute(cmdLine)
  end
end

-- Tests
function algorithmCompareTest(resolution, models, parms)
  resolution = resolution and resolution or {1024, 768}
  models = models and models or {path = testModels.path, testModels[1]} -- Cutter
  parms = parms and parms or 
    {
      {parameters.sphere_contrast, 60.0},
      {parameters.rayMarch_rfar, 0.08228},
      {parameters.rayMarch_contrast, 3.24},
      --Media
      {parameters.coneTracing_jitter, false,},
      {parameters.coneTracing_numSpheres, 6,},
      {parameters.coneTracing_numSamplers, 6,},
    }
  
  local tests = {
    parameters.timeTestEnabled,
    parameters.screenShotTestEnabled,
  }
  local algorithms = {
    parameters.sphereEnabled,
    parameters.rayMarchEnabled,
    parameters.coneTracingEnabled,
  }
    
  local sceneFullPath = workingDir..scenePath
  local logfileName = "AlgorithmCompareTest_"
  
  for i, model in ipairs(models) do
    local logName = logfileName .. model.name .. ".log"
    createSceneFile(sceneFullPath, resolution , {path = models.path, model})
    runTestProgram(logName, scenePath, tests, algorithms, parms)
    algorithmCompareLogAnaliser(logName)
  end
end

function parameterVariationTest(resolution, models)
  resolution = resolution and resolution or {1024, 768}
  models = models and models or {path = testModels.path, testModels[10]}
  -- models = models and models or testModels
    
  local tests = {
    parameters.timeTestEnabled,
    parameters.screenShotTestEnabled,
  }
  local algorithms = {
    parameters.coneTracingEnabled,
  }
  local varyingParms = {
    {
      default = { --Média
        {parameters.coneTracing_numSpheres, 6},
        {parameters.coneTracing_numSamplers, 6},
      },
      {parameters.coneTracing_jitter, {false, true},}
    },
  }
    
  local sceneFullPath = workingDir..scenePath
  local logfileName = "ParameterVariationTest_"
  
  
  for i, model in ipairs(models) do
    local logFileNameList = {}
    createSceneFile(sceneFullPath, resolution , {path = models.path, model})
    for j, varParms in ipairs(varyingParms) do
      for k, varParm in ipairs(varParms) do
        for l, varValue in ipairs(varParm[2]) do
          local parms = {}
          for m, defaultParm in ipairs(varParms.default) do
            table.insert(parms, defaultParm)
          end
          local logName = logfileName .. model.name .. "_" .. tostring(j) .. "_" .. tostring(k) .. "_" .. tostring(l) .. ".log"
          table.insert(parms, {varParm[1], varValue})
          runTestProgram(logName, scenePath, tests, algorithms, parms)
          table.insert(logFileNameList, logName)
        end
      end
    end
    parameterVariationLogAnaliser(logFileNameList)
  end
end

function scalabilityTest(models, parms)
  models = models and models or {path = testModels.path, testModels[1]}
  parms = parms and parms or 
      { --Média
        {parameters.coneTracing_jitter, false,},
        {parameters.coneTracing_numSpheres, 6,},
        {parameters.coneTracing_numSamplers, 6,},
      }

  local tests = {
    parameters.timeTestEnabled,
  }
  local algorithms = {
    parameters.coneTracingEnabled,
  }
  
  local resolutions = {
    {640, 480},
    {800, 600},
    {1024, 768},
    {1280, 960},
    {1600, 1200},
    {2048, 1536},
  }
    
  local sceneFullPath = workingDir..scenePath
  local logfileName = "ScalabilityTest_"
  
  for i, model in ipairs(models) do
    local logFileNameList = {}
    for j, resolution in ipairs(resolutions) do
      local logName = logfileName .. model.name .. "_" .. tostring(j) .. ".log"
      createSceneFile(sceneFullPath, resolution , {path = models.path, model})
      runTestProgram(logName, scenePath, tests, algorithms, parms)
      table.insert(logFileNameList, logName)
    end
    scalabilityLogAnaliser(logFileNameList)
  end
end

function screenshotQualityTest(resolution, models)
  resolution = resolution and resolution or {1024, 768}
  models = models and models or {path = testModels.path, testModels[1], testModels[4], testModels[5], testModels[8], testModels[11]}
    
  local tests = {
    parameters.timeTestEnabled,
    parameters.screenShotTestEnabled,
  }
  local algorithms = {
    parameters.coneTracingEnabled,
  }
  local varyingParms = {
    {
      name = "Baixa", 
      {parameters.coneTracing_jitter, false,},
      {parameters.coneTracing_numSpheres, 3,},
      {parameters.coneTracing_numSamplers, 3,},
    },    
    {
      name = "Media",
      {parameters.coneTracing_jitter, false,},
      {parameters.coneTracing_numSpheres, 6,},
      {parameters.coneTracing_numSamplers, 6,},
    },    
    {
      name = "Alta",
      {parameters.coneTracing_jitter, false,},
      {parameters.coneTracing_numSpheres, 6,},
      {parameters.coneTracing_numSamplers, 12,},
    },
  }
    
  local sceneFullPath = workingDir..scenePath
  local logfileName = "ScreenshotQualityTest_"
  
  for i, model in ipairs(models) do
    createSceneFile(sceneFullPath, resolution , {path = models.path, model})
    local logFileNameList = {}
    for j, parms in ipairs(varyingParms) do
      local logName = logfileName .. model.name .. "_" .. parms.name .. ".log"
      runTestProgram(logName, scenePath, tests, algorithms, parms)
      table.insert(logFileNameList, logName)
    end
    screenshotQualityLogAnaliser(logFileNameList)
  end
end

function timeTableTest(resolution, models, parms)
  resolution = resolution and resolution or {1024, 768}
  models = models and models or {path = testModels.path, testModels[1]}
  parms = parms and parms or 
      {
        {parameters.coneTracing_jitter, false,},
        {parameters.coneTracing_numSpheres, 6,},
        {parameters.coneTracing_numSamplers, 6,},
      }
  
  
  local tests = {
    parameters.timeTestEnabled,
    parameters.screenShotTestEnabled,
  }
  local algorithms = {
    parameters.coneTracingEnabled,
  }
  local sceneFullPath = workingDir..scenePath
  local logfileName = "TimeTableTest_"
  
  for i, model in ipairs(models) do
    local logName = logfileName .. model.name .. ".log"
    createSceneFile(sceneFullPath, resolution , {path = models.path, model})
    runTestProgram(logName, scenePath, tests, algorithms, parms)
    timeTableLogAnaliser(logName)
  end
end


-- Test Log Analiser
function algorithmCompareLogAnaliser(logFileName)
  local testName = "AlgorithmCompare"
  local timeMetric = "Mean" -- {"Mean" | "Single"}
  local logFile = loadfile(workingDir .. logFilePath .. logFileName)
  if logFile then
    local logFileTable = logFile()
    local modelName = logFileTable.scene.models[1][1]
    
    -- Create Result Log Dir
    local analiseLogDir = testName .. "_" .. modelName .. "_analysis"
    local cmdStr = string.gsub("mkdir " .. workingDir .. logFilePath .. analiseLogDir, "/", "\\")
    os.execute(cmdStr)
    
    -- Create Result Log Figure Dir
    local figuresDir = "figures"
    local cmdStr = string.gsub("mkdir " .. workingDir .. logFilePath .. analiseLogDir .. "/" .. figuresDir, "/", "\\")
    os.execute(cmdStr)
    
    -- Create Result Log File
    local texFileName = testName .. "_analysis.tex"
    local texFile = io.open(workingDir .. logFilePath .. analiseLogDir .. "/" .. texFileName, "w")
    
    --Intro
    texFile:write("%Tabela de comparação de tempo por algoritmo.\n\n")
    texFile:write("%Gerada em " .. logFileTable.creation_date .. "\n\n")
    texFile:write("\n")
    
    --Algorithms
    for i, alg in ipairs(logFileTable.algorithms) do
      texFile:write("%Algoritmo utilizado: " .. alg.name .. "\n\n")
      texFile:write("%Os parâmetros de execução são: \n")
      texFile:write("%\\begin{itemize}\n")
      for j, parm in ipairs(alg) do
        texFile:write("%  \\item " .. tostring(parm[1]) .. " = " .. tostring(parm[2]) .. "\n")
      end
      texFile:write("%\\end{itemize}\n")
    end
    texFile:write("\n\n")
    
    --Scene
    texFile:write("%A cena é composta pelo modelo ".. modelName ..".".."\n\n")
    -- texFile:write("O número total de vértices é ".. logFileTable.scene.totalVertices .." e de triângulos é ".. logFileTable.scene.totalTriangles.."\n\n")
    texFile:write("A cena é composta por " .. logFileTable.scene.totalVertices .. " vértices e ".. logFileTable.scene.totalTriangles.." triângulos.\n\n")
    texFile:write("%A resolução utilizada é ".. logFileTable.resolution[1] .."x" .. logFileTable.resolution[2].."\n\n")
    texFile:write("\n")

    -- Move Figures
    for i, alg in ipairs(logFileTable.algorithms) do
      for j, pose in ipairs(logFileTable[alg.name]) do
        local cmdLine = string.gsub("copy \"" .. workingDir .. pose.screenshotTest.screenshotPath .. pose.screenshotTest.screenshotFile .. "\"  "   
                                    .. workingDir .. logFilePath .. analiseLogDir .. "/" .. figuresDir .. "/" .. pose.screenshotTest.screenshotFile, "/", "\\")
        os.execute(cmdLine)
      end
    end
    
    -- SubFloats 
    texFile:write("\\begin{figure}[H]\n")
    texFile:write("  \\centering\n")
    
    for i, alg in ipairs(logFileTable.algorithms) do
      for j, pose in ipairs(logFileTable[alg.name]) do
        local n = #logFileTable[alg.name]
        local size = math.floor(.5 + 100/n)/100
        texFile:write("  \\subfloat{\\includegraphics[width=".. tostring(size) .."\\textwidth]{".. figuresDir .. "/" .. pose.screenshotTest.screenshotFile .. "} }" .. (j == n and "\\\\" or "") .. "\n")
      end
      texFile:write("  " .. alg.name .. " \\\\ \n")
      texFile:write("  \\vspace{15pt} \n")
    end
    
    for j, pose in ipairs(logFileTable.poses) do
      local n = #logFileTable.poses
      local size = math.floor(.5 + 100/n)/100
      texFile:write("  \\subfloat{\\includegraphics[width=".. tostring(size) .."\\textwidth]{".. figuresDir .. "/" .. modelName .. pose .. ".png" .. "} }" .. (j == n and "\\\\" or "") .. "\n")
    end
    texFile:write("  Optix \\\\ \n")

    texFile:write("  \\caption{Compração entre os algoritmos.}\n")
    texFile:write("  \\label{fig:".. testName .."}\n")
    texFile:write("\\end{figure}\n")
    texFile:write("\n")
    texFile:write("\n")
    
    
    --TableLines
    local tableLines = {}
    for i, alg in ipairs(logFileTable.algorithms) do
      local tableLine = alg.name
      for j, pose in ipairs(logFileTable[alg.name]) do
        tableLine = tableLine .. " & " .. tostring(math.floor(pose.timeTest.total.GPU[timeMetric]*100+.5)/100) .. " ms"
      end
      tableLine = tableLine .. " \\\\ \n"
      for j, pose in ipairs(logFileTable[alg.name]) do
        local ms = math.floor(pose.timeTest.total.GPU[timeMetric]*100+.5)/100
        local fps = math.floor((1.0/(ms/1000.0))*100 + .5)/100
        tableLine = tableLine .. " & " .. tostring(fps) .. " fps"
      end
      table.insert(tableLines, tableLine)
    end
    
    -- Table
    local columns = " c |"
    for i, j in ipairs(logFileTable.poses) do
      columns = columns .. " c |"
    end
    
    texFile:write("%A tabela a seguir apresenta os resultados obtidos para cada algoritmo." .. "\n")
    texFile:write("\\begin{table}[H]".."\n")
    texFile:write("  \\centering".."\n")
    texFile:write("  \\caption{Resultados de tempo para cada aloritmo.}".."\n")
    texFile:write("  \\label{tab:".. testName .."}\n")
    texFile:write("  \\begin{tabular}{ |"..columns.."}".."\n")
    texFile:write("    \\hline".."\n")
    --Header
    texFile:write("  Algoritmo")
    for i, pose in ipairs(logFileTable.poses) do
      texFile:write(" & Pose " .. i .. " " )
    end
    texFile:write("\\\\ \n")
    texFile:write("    \\hline".."\n")
    
    --Content
    for i, tabLine in ipairs(tableLines) do
      texFile:write("    " .. tabLine .. " \\\\ \n")
      texFile:write("    \\hline".."\n")
    end
    
    texFile:write("  \\end{tabular}".."\n")
    texFile:write("\\end{table}".."\n")
    texFile:write("\n")
    texFile:close()
  end
end

function parameterVariationLogAnaliser(logFileNameList)
  local testName = "ParameterVariation"
  local timeMetric = "Mean" -- {"Mean" | "Single"}
  
  local logFileList = {}
  for i, logFileName in ipairs(logFileNameList) do
    local logFile = loadfile(workingDir .. logFilePath .. logFileName)
    table.insert(logFileList, logFile)
  end
  
  local logFileTableList = {}
  for i, logFile in ipairs(logFileList) do
    local logFileTable = logFile()
    table.insert(logFileTableList, logFileTable)
  end  
  
  local logFileTable = logFileTableList[1]
  local modelName = logFileTable.scene.models[1][1]
    
  -- Create Result Log Dir
  local analiseLogDir = testName .. "_" .. modelName .. "_analysis"
  local cmdStr = string.gsub("mkdir " .. workingDir .. logFilePath .. analiseLogDir, "/", "\\")
  os.execute(cmdStr)
    
  -- Create Result Log Figure Dir
  local figuresDir = "figures"
  local cmdStr = string.gsub("mkdir " .. workingDir .. logFilePath .. analiseLogDir .. "/" .. figuresDir, "/", "\\")
  os.execute(cmdStr)
    
  -- Create Result Log File
  local texFileName = testName .. "_analysis.tex"
  local texFile = io.open(workingDir .. logFilePath .. analiseLogDir .. "/" .. texFileName, "w")
  
  --Intro
  texFile:write("%Tabela de comparação de tempo de acordo com cada parâmetro.\n\n")
  texFile:write("%Gerada em " .. logFileTable.creation_date .. "\n\n")
  texFile:write("\n")
  
  --Algorithms
  for i, alg in ipairs(logFileTable.algorithms) do
    texFile:write("%Algoritmo utilizado: " .. alg.name .. "\n\n")
  end
  texFile:write("\n")
    
  --Scene
  texFile:write("%A cena é composta pelo modelo ".. modelName ..".".."\n\n")
  texFile:write("A cena é composta por " .. logFileTable.scene.totalVertices .. " vértices e ".. logFileTable.scene.totalTriangles.." triângulos.\n\n")
  texFile:write("%A resolução utilizada é ".. logFileTable.resolution[1] .."x" .. logFileTable.resolution[2].."\n\n")
  texFile:write("\n")

  -- Move Figures
  for i, logFileTable in ipairs(logFileTableList) do
    for j, pose in ipairs(logFileTable[logFileTable.algorithms[1].name]) do
      local cmdLine = string.gsub("copy \"" .. workingDir .. pose.screenshotTest.screenshotPath .. pose.screenshotTest.screenshotFile .. "\"  "   
                                  .. workingDir .. logFilePath .. analiseLogDir .. "/" .. figuresDir .. "/" .. pose.screenshotTest.screenshotFile, "/", "\\")
      os.execute(cmdLine)
    end
  end
  
  --Instance Text
  local nameLines = {"\\emph{Jitter} desativado","\\emph{Jitter} ativado"}
  -- local parmList = {3, 6, 7} --jitter, numSpheres, numSamplers
  -- for i, logFileTable in ipairs(logFileTableList) do
    -- local alg = logFileTable.algorithms[1]
    -- local instanceLine = ""
    -- for j, parmIndex in ipairs(parmList) do
      -- instanceLine = instanceLine .. alg[parmIndex][1] .. "(" .. alg[parmIndex][2] .. ")" .. (j ~= #parmList and ", " or "")
    -- end
    -- table.insert(nameLines, instanceLine)
  -- end
  
  -- SubFloats 
  for i, logFileTable in ipairs(logFileTableList) do
    texFile:write("\\begin{figure}[H]\n")
    texFile:write("  \\centering\n")
    local alg = logFileTable.algorithms[1]
    local pose = logFileTable[alg.name][1]
    local n = #logFileTable[alg.name]
    local size = math.min(math.floor(.5 + 100/n)/100, .5)
    texFile:write("  \\subfloat{\\includegraphics[width=".. tostring(size) .."\\textwidth]{".. figuresDir .. "/" .. pose.screenshotTest.screenshotFile .. "} }" .. "\n")
    texFile:write("\\\\  " .. nameLines[i] .. " \n")
    texFile:write("\\end{figure}\n")
    texFile:write("\n")
  end
  
  texFile:write("\\begin{figure}[H]\n")
  texFile:write("  \\centering\n")
  local pose = logFileTable.poses[1]
  local n = #logFileTable.poses
  local size = math.min(math.floor(.5 + 100/n)/100, .5)
  texFile:write("  \\subfloat{\\includegraphics[width=".. tostring(size) .."\\textwidth]{".. figuresDir .. "/" .. modelName .. pose .. ".png" .. "} }" .. "\n")
  texFile:write("\\\\    Optix \n")
  texFile:write("    \\caption{Compração entre execução com e sem \\emph{jitter}.} \n")
  texFile:write("    \\label{fig:".. testName .."}\n")
  texFile:write("\\end{figure}\n")
  texFile:write("\n")
  texFile:write("\n")
  
  
  --TableLines
  local tableLines = {}
  for i, logFileTable in ipairs(logFileTableList) do
    local alg = logFileTable.algorithms[1]
    local tableLine = nameLines[i]
    local pose = logFileTable[logFileTable.algorithms[1].name][1]
    local ms = math.floor(pose.timeTest.total.GPU[timeMetric]*100+.5)/100
    local fps = math.floor((1.0/(ms/1000.0))*100 + .5)/100
    tableLine = tableLine .. " & " .. tostring(ms)  
    tableLine = tableLine .. " & " .. tostring(fps) 
    table.insert(tableLines, tableLine)
  end
 
  
  --Table
  local columns = " c | c | c |"
  -- for i, j in ipairs(logFileTable.poses) do
    -- columns = columns .. " c |"
  -- end
  
  texFile:write("%A tabela a seguir apresenta os resultados obtidos para cada varialão de jitter." .. "\n")
  texFile:write("\\begin{table}[H]".."\n")
  texFile:write("  \\centering".."\n")
  texFile:write("  \\caption{Resultados de tempo para a execução com e sem \\emph{jitter}.}".."\n")
  texFile:write("  \\label{tab:".. testName .."}\n")
  texFile:write("  \\begin{tabular}{ |"..columns.."}".."\n")
  texFile:write("    \\hline".."\n")
  --Header
  texFile:write("    Parâmetros & Tempo (ms) & FPS")

  texFile:write("\\\\ \n")
  texFile:write("    \\hline".."\n")
  
  --Content
  for i, tabLine in ipairs(tableLines) do
    texFile:write("    " .. tabLine .. " \\\\ \n")
    texFile:write("    \\hline".."\n")
  end
  
  texFile:write("  \\end{tabular}".."\n")
  texFile:write("\\end{table}".."\n")
  texFile:write("\n")
  texFile:close()
end

function scalabilityLogAnaliser(logFileNameList)
  local testName = "Scalability"
  local timeMetric = "Mean" -- {"Mean" | "Single"}
  
  local logFileList = {}
  for i, logFileName in ipairs(logFileNameList) do
    local logFile = loadfile(workingDir .. logFilePath .. logFileName)
    table.insert(logFileList, logFile)
  end
  
  local logFileTableList = {}
  for i, logFile in ipairs(logFileList) do
    local logFileTable = logFile()
    table.insert(logFileTableList, logFileTable)
  end  
  
  local logFileTable = logFileTableList[1]
  local modelName = logFileTable.scene.models[1][1]
    
  -- Create Result Log Dir
  local analiseLogDir = testName .. "_" .. modelName .. "_analysis"
  local cmdStr = string.gsub("mkdir " .. workingDir .. logFilePath .. analiseLogDir, "/", "\\")
  os.execute(cmdStr)
    
  -- Create Result Log File
  local texFileName = testName .. "_analysis.tex"
  local texFile = io.open(workingDir .. logFilePath .. analiseLogDir .. "/" .. texFileName, "w")
    
  --Intro
  texFile:write("%Tabela de comparação de tempo de acordo com cada resolução.\n\n")
  texFile:write("%Gerada em " .. logFileTable.creation_date .. "\n\n")
  texFile:write("\n")
  
  --Algorithms
  for i, alg in ipairs(logFileTable.algorithms) do
    texFile:write("%Algoritmo utilizado: " .. alg.name .. "\n\n")
  end
  texFile:write("\n")
    
  --Scene
  texFile:write("%A cena é composta pelo modelo ".. modelName ..".".."\n\n")
  texFile:write("A cena é composta por " .. logFileTable.scene.totalVertices .. " vértices e ".. logFileTable.scene.totalTriangles.." triângulos.\n\n")
  texFile:write("\n")
  
  --TableLines
  local csvTableLines = {}
  local parameterTableLines = {}
  
  for i, logFileTable in ipairs(logFileTableList) do
    local alg = logFileTable.algorithms[1]
    local pose = logFileTable[alg.name][1]
    local tableLine = logFileTable.resolution[1] .. "x" .. logFileTable.resolution[2]
    local csvLine = "%%" .. logFileTable.resolution[1]*logFileTable.resolution[2]
    
    local ms = math.floor(pose.timeTest.total.GPU[timeMetric]*100+.5)/100
    local fps = math.floor((1.0/(ms/1000.0))*100 + .5)/100
    tableLine = tableLine .. " & " .. tostring(ms) 
    csvLine = csvLine .. "\t" .. tostring(ms)
    tableLine = tableLine .. " & " .. tostring(fps)
    
    table.insert(parameterTableLines, tableLine)
    table.insert(csvTableLines, csvLine)
  end

  --Table
  local columns = " c | c | c |"

  texFile:write("A tabela a seguir apresenta os resultados obtidos para cada resolução." .. "\n")
  texFile:write("\\begin{table}[H]".."\n")
  texFile:write("  \\centering".."\n")
  texFile:write("  \\caption{Resultados de tempo para a execução em diversas resoluções.}\n")
  texFile:write("  \\label{tab:".. testName .."}\n")
  texFile:write("  \\begin{tabular}{ |"..columns.."}".."\n")
  texFile:write("    \\hline".."\n")
  --Header
  texFile:write("    Resolução & Tempo (ms) & FPS \\\\ \n")
  texFile:write("    \\hline".."\n")
  
  --Content
  for i, parameterLine in ipairs(parameterTableLines) do
    texFile:write("    " .. parameterLine .. " \\\\ \n")
    texFile:write("    \\hline".."\n")
  end
  
  texFile:write("  \\end{tabular}".."\n")
  texFile:write("\\end{table}".."\n")
  
  texFile:write("%%CSV Data".."\n")
  
  for i, csvLine in ipairs(csvTableLines) do
    texFile:write(csvLine .. "\n")
  end
  
  texFile:close()
  
end

function screenshotQualityLogAnaliser(logFileNameList)
  local testName = "ScreenshotQuality"
  local timeMetric = "Mean" -- {"Mean" | "Single"}
  
  local logFileList = {}
  for i, logFileName in ipairs(logFileNameList) do
    local logFile = loadfile(workingDir .. logFilePath .. logFileName)
    table.insert(logFileList, logFile)
  end
  
  local logFileTableList = {}
  for i, logFile in ipairs(logFileList) do
    local logFileTable = logFile()
    table.insert(logFileTableList, logFileTable)
  end  
  
  local logFileTable = logFileTableList[1]
  local modelName = logFileTable.scene.models[1][1]
    
  -- Create Result Log Dir
  local analiseLogDir = testName .. "_" .. modelName .. "_analysis"
  local cmdStr = string.gsub("mkdir " .. workingDir .. logFilePath .. analiseLogDir, "/", "\\")
  os.execute(cmdStr)
    
  -- Create Result Log Figure Dir
  local figuresDir = "figures"
  local cmdStr = string.gsub("mkdir " .. workingDir .. logFilePath .. analiseLogDir .. "/" .. figuresDir, "/", "\\")
  os.execute(cmdStr)
    
  -- Create Result Log File
  local texFileName = testName .. "_" .. modelName .. "_analysis.tex"
  local texFile = io.open(workingDir .. logFilePath .. analiseLogDir .. "/" .. texFileName, "w")

  texFile:write("\\subsection{Modelo ".. modelName .."}\n\n")
  --Intro
  texFile:write("%Tabela de comparação qualidade.\n\n")
  texFile:write("%Gerada em " .. logFileTable.creation_date .. "\n\n")
  texFile:write("\n")
  
  --Algorithms
  for i, alg in ipairs(logFileTable.algorithms) do
    texFile:write("%Algoritmo utilizado: " .. alg.name .. "\n\n")
  end
  texFile:write("\n")
    
  --Scene
  texFile:write("%A cena é composta pelo modelo ".. modelName ..".".."\n\n")
  texFile:write("A cena é composta por " .. logFileTable.scene.totalVertices .. " vértices e ".. logFileTable.scene.totalTriangles.." triângulos.\n\n")
  texFile:write("%A resolução utilizada é ".. logFileTable.resolution[1] .."x" .. logFileTable.resolution[2].."\n\n")
  texFile:write("\n")
  
  -- Move Figures
  for i, logFileTable in ipairs(logFileTableList) do
    for j, pose in ipairs(logFileTable[logFileTable.algorithms[1].name]) do
      local cmdLine = string.gsub("copy \"" .. workingDir .. pose.screenshotTest.screenshotPath .. pose.screenshotTest.screenshotFile .. "\"  "   
                                  .. workingDir .. logFilePath .. analiseLogDir .. "/" .. figuresDir .. "/" .. pose.screenshotTest.screenshotFile, "/", "\\")
      os.execute(cmdLine)
    end
  end
  
  local nameLines = {"Baixa", "Média", "Alta"}
  -- SubFloats 
  for i, logFileTable in ipairs(logFileTableList) do
    texFile:write("\\begin{figure}[H]\n")
    texFile:write("  \\centering\n")
    local alg = logFileTable.algorithms[1]
    for j, pose in ipairs(logFileTable[alg.name]) do
      local n = #logFileTable[alg.name]
      local size = math.min(math.floor(.5 + 100/n)/100, .5)
      texFile:write("  \\subfloat{\\includegraphics[width=".. tostring(size) .."\\textwidth]{".. figuresDir .. "/" .. pose.screenshotTest.screenshotFile .. "} }" .. (j == n and "\\\\" or "") .. "\n")
    end
    texFile:write("  " .. nameLines[i] .. "\n")
    texFile:write("\\end{figure}\n")
  end
  
  texFile:write("\\begin{figure}[H]\n")
  texFile:write("  \\centering\n")
  for j, pose in ipairs(logFileTable.poses) do
    local n = #logFileTable.poses
    local size = math.min(math.floor(.5 + 100/n)/100, .5)
    texFile:write("  \\subfloat{\\includegraphics[width=".. tostring(size) .."\\textwidth]{".. figuresDir .. "/" .. modelName .. pose .. ".png" .. "} }" .. (j == n and "\\\\" or "") .. "\n")
  end
  texFile:write("  Optix \\\\ \n")

  texFile:write("  \\caption{Compração entre diferentes configurações de qualidade.}\n")
  texFile:write("\\end{figure}\n")
  texFile:write("\n")
  texFile:write("\n")
  
  
  --TableLines
  local parameterTableLines = {}
  for i, logFileTable in ipairs(logFileTableList) do
    local alg = logFileTable.algorithms[1]
    local tableLine = nameLines[i]
    for j, pose in ipairs(logFileTable[logFileTable.algorithms[1].name]) do
      tableLine = tableLine .. " & " .. tostring(math.floor(pose.timeTest.total.GPU[timeMetric]*100+.5)/100) .. " ms"
    end
    tableLine = tableLine .. " \\\\ \n"
    for j, pose in ipairs(logFileTable[logFileTable.algorithms[1].name]) do
      local ms = math.floor(pose.timeTest.total.GPU[timeMetric]*100+.5)/100
      local fps = math.floor((1.0/(ms/1000.0))*100 + .5)/100
      tableLine = tableLine .. " & " .. tostring(fps) .. " fps"
    end
    table.insert(parameterTableLines, tableLine)
  end

  -- Table
  local columns = " p{2cm} |"
  for i, j in ipairs(logFileTable.poses) do
    columns = columns .. " c |"
  end
  
  texFile:write("%A tabela a seguir apresenta os resultados obtidos para cada configuração de qualidade." .. "\n")
  texFile:write("\\begin{table}[H]".."\n")
  texFile:write("  \\centering".."\n")
  texFile:write("  \\caption{Compração entre execuções de diferentes qualidades do modelo " .. modelName .. ".}".."\n")
  texFile:write("  \\begin{tabular}{ |"..columns.."}".."\n")
  texFile:write("    \\hline".."\n")
  --Header
  texFile:write("    Qualidade")
  for i, pose in ipairs(logFileTable.poses) do
    texFile:write(" & Pose " .. i)
  end
  texFile:write("    \\\\ \n")
  texFile:write("    \\hline".."\n")
  
  --Content
  for i, parameterLine in ipairs(parameterTableLines) do
    texFile:write("    " .. parameterLine .. " \\\\ \n")
    texFile:write("    \\hline".."\n")
  end
  
  texFile:write("  \\end{tabular}".."\n")
  texFile:write("\\end{table}".."\n")
  texFile:close()
  
end

function timeTableLogAnaliser(logFileName)
  local testName = "TimeTable"
  local timeMetric = "Mean" -- {"Mean" | "Single"}
  local logFile = loadfile(workingDir .. logFilePath .. logFileName)
  if logFile then
    local logFileTable = logFile()
    local modelName = logFileTable.scene.models[1][1]
    
    -- Create Result Log Dir
    local analiseLogDir = testName .. "_" .. modelName .. "_analysis"
    local cmdStr = string.gsub("mkdir " .. workingDir .. logFilePath .. analiseLogDir, "/", "\\")
    os.execute(cmdStr)
    
    -- Create Result Log File
    local texFileName = testName .. "_analysis.tex"
    local texFile = io.open(workingDir .. logFilePath .. analiseLogDir .. "/" .. texFileName, "w")
    
    --Intro
    texFile:write("%Tabela de comparação de tempo por etapas.\n\n")
    texFile:write("%Gerada em " .. logFileTable.creation_date .. "\n\n")
    texFile:write("\n")
    --Algorithms
    for i, alg in ipairs(logFileTable.algorithms) do
      texFile:write("%Algoritmo utilizado: " .. alg.name .. "\n\n")
      texFile:write("%Os parâmetros de execução são: \n")
      texFile:write("%\\begin{itemize}\n")
      for j, parm in ipairs(alg) do
        texFile:write("%  \\item " .. tostring(parm[1]) .. " = " .. tostring(parm[2]) .. "\n")
      end
      texFile:write("%\\end{itemize}\n")
    end
    texFile:write("\n")
    
    --Scene
    texFile:write("%A cena é composta pelo modelo ".. modelName ..".".."\n\n")
    texFile:write("A cena é composta por " .. logFileTable.scene.totalVertices .. " vértices e ".. logFileTable.scene.totalTriangles.." triângulos.\n\n")
    texFile:write("%A resolução utilizada é ".. logFileTable.resolution[1] .."x" .. logFileTable.resolution[2].."\n\n")
    texFile:write("\n")
    
    --TableLines
    local tableLines = {}
    local pose = logFileTable[logFileTable.algorithms[1].name][1]
    for i, step in ipairs(pose.timeTest.steps) do
      local tableLine = step.name
      local ms = math.floor(pose.timeTest.steps[i].GPU[timeMetric]*100+.5)/100
      local fps = math.floor((1.0/(ms/1000.0))*100 + .5)/100
      local totalMean = pose.timeTest.total.GPU[timeMetric]
      local percent = math.floor((1000*pose.timeTest.steps[i].GPU[timeMetric]/totalMean)+.5)/10
      tableLine = tableLine .. " & " .. tostring(ms) 
      tableLine = tableLine .. " & " .. tostring(fps) 
      tableLine = tableLine .. " & " .. tostring(percent) .. " \\%"
      table.insert(tableLines, tableLine)
    end
    local tableLine = "Total"
    local ms = math.floor(pose.timeTest.total.GPU[timeMetric]*100+.5)/100
    local fps = math.floor((1.0/(ms/1000.0))*100 + .5)/100
    tableLine = tableLine .. " & " .. tostring(ms)
    tableLine = tableLine .. " & " .. tostring(fps) 
    tableLine = tableLine .. " & --"
    table.insert(tableLines, tableLine)
    
    
    -- Table
    local columns = " c | c | c | c |"

    
    texFile:write("%A tabela a seguir apresenta os resultados obtidos para cada etapa." .. "\n")
    texFile:write("\\begin{table}[H]".."\n")
    texFile:write("  \\centering".."\n")
    texFile:write("  \\caption{Resultados de tempo para a execução de cada uma das etapas.}".."\n")
    texFile:write("  \\label{tab:".. testName .."}\n")
    texFile:write("  \\begin{tabular}{ |"..columns.."}".."\n")
    texFile:write("    \\hline".."\n")
    --Header
    texFile:write("   Etapa & Tempo (ms) & FPS & Percentual")
    
    texFile:write("  \\\\ \n")
    texFile:write("    \\hline".."\n")
    
    --Content
    for i, tabLine in ipairs(tableLines) do
      texFile:write("    " .. tabLine .. " \\\\ \n")
      texFile:write("    \\hline".."\n")
    end
    
    texFile:write("  \\end{tabular}".."\n")
    texFile:write("\\end{table}".."\n")
    texFile:close()
  end
end

------------------------------
------------------------------
------------------------------
------------------------------
------------------------------
------------------------------
------------------------------
------------------------------
------------------------------
------------------------------
if activeTests.timeTable then  
  timeTableTest()
end

if activeTests.parameterVariation  then  
  parameterVariationTest()
end  

if activeTests.screenshotQuality  then  
  screenshotQualityTest()
end

if activeTests.scalability  then  
  scalabilityTest()
end

if activeTests.algorithmCompare then  
  algorithmCompareTest()
end







