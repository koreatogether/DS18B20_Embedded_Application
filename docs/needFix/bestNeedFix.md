Why this is a problem
Write-Host outputs directly to the host and does not write to the pipeline, making its output unpredictable and unusable for further processing or redirection. Prior to PowerShell 5.0, Write-Host did not write to any stream, limiting control over its output. Using Write-Output or Write-Verbose ensures output can be managed properly.

How to fix it
Replace Write-Host with Write-Output to send data to the pipeline or Write-Verbose for informational messages that can be optionally displayed by the user. Only use Write-Host in functions with the Show verb, where displaying information directly to the user is intended.

Examples
Good examples
function Get-MeaningOfLife
{
    [CmdletBinding()]Param() # makes it possible to support Verbose output

    Write-Verbose 'Computing the answer to the ultimate question of life, the universe and everything'
    Write-Output 42
}

function Show-Something
{
    Write-Host 'show something on screen'
}
Bad examples
function Get-MeaningOfLife
{
    Write-Host 'Computing the answer to the ultimate question of life, the universe and everything'
    Write-Host 42
}




--------------------



---

#### [☑️] monitor_quality.ps1의 Write-Host 사용 문제 목록

1. ☑️ Write-Host는 모든 호스트에서 동작하지 않을 수 있고, 호스트가 없으면 동작하지 않음
2. ☑️ PowerShell 5.0 이전에는 Write-Host 출력을 억제, 캡처, 리디렉션할 수 없음
3. ☑️ Write-Host는 파이프라인에 데이터를 전달하지 않아, 후속 처리/자동화에 부적합함
4. ☑️ Write-Output, Write-Verbose, Write-Information을 사용하면 출력 관리가 쉬워짐
5. ☑️ monitor_quality.ps1에서 Write-Host를 다수 사용하고 있으므로, 위 함수들로 대체 필요