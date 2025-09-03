$ErrorActionPreference = "Stop"

# region 项目参数

$project_name = "stm32h743-project"
$platform = "arm-none-eabi-cortex-m7"
$cmake_config = "arm-none-eabi-cortex-m7-debug"

# endregion



# region 计算所得的项目参数。不需要填写。

$workspace_path = Get-Location
$build_path = "$workspace_path/jc_build"
$install_path = "$env:cpp_lib_build_scripts_path/${platform}/.libs/${project_name}"

Write-Host "构建路径 ${build_path}"
Write-Host "安装路径 ${install_path}"

# endregion


# region 如果还没编译安装，就先编译安装。

New-Item -Path $build_path -ItemType Directory -Force
Push-Location $build_path
try
{
	cmake -G "Ninja" $workspace_path `
		--preset "$cmake_config"

	if ($LASTEXITCODE)
	{
		throw "配置失败"
	}

	ninja -j12
	if ($LASTEXITCODE)
	{
		throw "编译失败"
	}

	ninja install
}
finally
{
	Pop-Location
}

# endregion
