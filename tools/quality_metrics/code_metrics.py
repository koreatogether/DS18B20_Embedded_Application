#!/usr/bin/env python3
"""
DS18B20 Embedded Application - Code Quality Metrics Monitor
==========================================================

코드 품질 메트릭을 수집하고 모니터링하는 도구입니다.
- 코드 라인 수 (Lines of Code)
- 함수/클래스 복잡도
- 테스트 커버리지 추정
- 의존성 분석
- 아키텍처 준수도 검사
"""

import re
import json
from datetime import datetime
from pathlib import Path
from typing import Dict, Tuple, Any

class CodeMetricsCollector:
    def __init__(self, project_root: str):
        self.project_root = Path(project_root)
        self.src_dir = self.project_root / "src"
        self.test_dir = self.project_root / "test"
        self.docs_dir = self.project_root / "docs"
        
        # 메트릭 결과 저장소
        self.metrics = {
            "timestamp": datetime.now().isoformat(),
            "code_metrics": {},
            "architecture_metrics": {},
            "test_metrics": {},
            "build_metrics": {},
            "quality_score": 0.0
        }

    def collect_code_metrics(self) -> Dict[str, Any]:
        """코드 메트릭 수집 (라인 수, 복잡도 등)"""
        print("📊 Collecting code metrics...")
        
        metrics = {
            "total_lines": 0,
            "source_files": 0,
            "header_files": 0,
            "test_files": 0,
            "functions": 0,
            "classes": 0,
            "complexity_score": 0.0,
            "files_breakdown": {}
        }
        
        # 소스 파일 분석
        for ext in ['*.cpp', '*.h']:
            for file_path in self.src_dir.rglob(ext):
                if file_path.is_file():
                    file_metrics = self._analyze_file(file_path)
                    metrics["total_lines"] += file_metrics["lines"]
                    metrics["functions"] += file_metrics["functions"]
                    metrics["classes"] += file_metrics["classes"]
                    metrics["complexity_score"] += file_metrics["complexity"]
                    
                    if ext == '*.cpp':
                        metrics["source_files"] += 1
                    else:
                        metrics["header_files"] += 1
                    
                    metrics["files_breakdown"][str(file_path.relative_to(self.project_root))] = file_metrics
        
        # 테스트 파일 분석
        for file_path in self.test_dir.rglob("*.cpp"):
            if file_path.is_file():
                file_metrics = self._analyze_file(file_path)
                metrics["test_files"] += 1
                metrics["files_breakdown"][str(file_path.relative_to(self.project_root))] = file_metrics
        
        # 평균 복잡도 계산
        if metrics["functions"] > 0:
            metrics["avg_complexity"] = metrics["complexity_score"] / metrics["functions"]
        else:
            metrics["avg_complexity"] = 0.0
            
        return metrics

    def _analyze_file(self, file_path: Path) -> Dict[str, Any]:
        """개별 파일 분석"""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
        except UnicodeDecodeError:
            try:
                with open(file_path, 'r', encoding='latin-1') as f:
                    content = f.read()
            except Exception:
                return {"lines": 0, "functions": 0, "classes": 0, "complexity": 0}
        
        lines = len(content.splitlines())
        
        # 함수 개수 (간단한 패턴 매칭)
        function_pattern = r'^\s*(?:virtual\s+)?(?:static\s+)?(?:inline\s+)?(?:\w+\s+)*\w+\s*\([^)]*\)\s*(?:const\s*)?(?:override\s*)?[{;]'
        functions = len(re.findall(function_pattern, content, re.MULTILINE))
        
        # 클래스 개수
        class_pattern = r'^\s*class\s+\w+'
        classes = len(re.findall(class_pattern, content, re.MULTILINE))
        
        # 간단한 복잡도 (if/for/while/switch 문 개수)
        complexity_patterns = [r'\bif\s*\(', r'\bfor\s*\(', r'\bwhile\s*\(', r'\bswitch\s*\(']
        complexity = sum(len(re.findall(pattern, content)) for pattern in complexity_patterns)
        
        return {
            "lines": lines,
            "functions": functions,
            "classes": classes,
            "complexity": complexity
        }

    def collect_architecture_metrics(self) -> Dict[str, Any]:
        """아키텍처 메트릭 수집 (Clean Architecture 준수도)"""
        print("🏗️ Collecting architecture metrics...")
        
        metrics = {
            "layer_separation": self._check_layer_separation(),
            "dependency_inversion": self._check_dependency_inversion(),
            "interface_usage": self._check_interface_usage(),
            "solid_principles": self._check_solid_principles(),
            "architecture_score": 0.0
        }
        
        # 아키텍처 점수 계산 (0-100)
        scores = [
            metrics["layer_separation"]["score"],
            metrics["dependency_inversion"]["score"],
            metrics["interface_usage"]["score"],
            metrics["solid_principles"]["score"]
        ]
        metrics["architecture_score"] = sum(scores) / len(scores)
        
        return metrics

    def _check_layer_separation(self) -> Dict[str, Any]:
        """계층 분리 확인"""
        domain_files = list((self.src_dir / "domain").glob("*.h")) if (self.src_dir / "domain").exists() else []
        application_files = list((self.src_dir / "application").glob("*.h")) if (self.src_dir / "application").exists() else []
        infrastructure_files = list((self.src_dir / "infrastructure").glob("*.h")) if (self.src_dir / "infrastructure").exists() else []
        
        total_files = len(domain_files) + len(application_files) + len(infrastructure_files)
        
        return {
            "domain_files": len(domain_files),
            "application_files": len(application_files),
            "infrastructure_files": len(infrastructure_files),
            "total_layered_files": total_files,
            "score": min(100.0, total_files * 10)  # 파일 수에 따른 점수
        }

    def _check_dependency_inversion(self) -> Dict[str, Any]:
        """의존성 역전 원칙 확인"""
        interfaces = []
        implementations = []
        
        # 인터페이스 파일 찾기 (I로 시작하는 클래스)
        for file_path in self.src_dir.rglob("*.h"):
            if file_path.is_file():
                try:
                    with open(file_path, 'r', encoding='utf-8') as f:
                        content = f.read()
                    
                    # 인터페이스 패턴 찾기
                    if re.search(r'class\s+I[A-Z]\w+', content):
                        interfaces.append(str(file_path.name))
                    
                    # 구현 클래스 패턴 찾기 (인터페이스를 상속받는)
                    if re.search(r'class\s+\w+\s*:\s*public\s+I[A-Z]\w+', content):
                        implementations.append(str(file_path.name))
                        
                except Exception:
                    continue
        
        dip_score = 0.0
        if len(interfaces) > 0:
            dip_score = min(100.0, (len(implementations) / len(interfaces)) * 100)
        
        return {
            "interfaces_count": len(interfaces),
            "implementations_count": len(implementations),
            "interfaces": interfaces,
            "score": dip_score
        }

    def _check_interface_usage(self) -> Dict[str, Any]:
        """인터페이스 사용률 확인"""
        # Mock 파일 개수
        mock_files = list(self.test_dir.rglob("Mock*.h")) if self.test_dir.exists() else []
        
        return {
            "mock_files_count": len(mock_files),
            "mock_files": [f.name for f in mock_files],
            "score": min(100.0, len(mock_files) * 15)  # Mock 파일 수에 따른 점수
        }

    def _check_solid_principles(self) -> Dict[str, Any]:
        """SOLID 원칙 준수도 확인"""
        # 단순화된 SOLID 체크 (파일 크기 및 책임 분리)
        large_files = []
        total_files = 0
        
        for file_path in self.src_dir.rglob("*.cpp"):
            if file_path.is_file():
                total_files += 1
                try:
                    with open(file_path, 'r', encoding='utf-8') as f:
                        lines = len(f.readlines())
                    if lines > 200:  # 200라인 이상은 큰 파일로 간주
                        large_files.append({"file": str(file_path.name), "lines": lines})
                except Exception:
                    continue
        
        # 큰 파일이 적을수록 높은 점수
        score = 100.0
        if total_files > 0:
            large_file_ratio = len(large_files) / total_files
            score = max(0.0, 100.0 - (large_file_ratio * 100))
        
        return {
            "total_source_files": total_files,
            "large_files_count": len(large_files),
            "large_files": large_files,
            "score": score
        }

    def collect_test_metrics(self) -> Dict[str, Any]:
        """테스트 메트릭 수집"""
        print("🧪 Collecting test metrics...")
        
        # 테스트 실행 결과 파싱
        test_log_path = self.project_root / "test" / "logs" / "test_results_clean.txt"
        
        metrics = {
            "test_files": 0,
            "test_cases": 0,
            "passed_tests": 0,
            "failed_tests": 0,
            "test_success_rate": 0.0,
            "execution_time": 0.0,
            "coverage_estimate": 0.0
        }
        
        # 테스트 파일 개수
        test_files = list(self.test_dir.rglob("test_*.cpp")) if self.test_dir.exists() else []
        metrics["test_files"] = len(test_files)
        
        # 최근 테스트 결과 읽기
        if test_log_path.exists():
            try:
                # 여러 인코딩 방식 시도 (UTF-16 BOM 포함)
                for encoding in ['utf-16', 'utf-8', 'latin-1', 'cp1252']:
                    try:
                        with open(test_log_path, 'r', encoding=encoding) as f:
                            content = f.read()
                        print(f"✅ Successfully decoded with {encoding}")
                        break
                    except UnicodeDecodeError:
                        continue
                else:
                    print("❌ Could not decode test log file")
                    return metrics
                
                print(f"📋 Reading test log: {test_log_path}")
                print(f"📄 Content preview: {content[:500]}")  # 처음 500글자 출력
                
                # Unity 테스트 결과 파싱 (PlatformIO 형식)
                # "16 Tests 0 Failures 0 Ignored" 패턴 찾기
                test_match = re.search(r'(\d+) Tests (\d+) Failures (\d+) Ignored', content)
                print(f"🔍 Regex match result: {test_match}")
                
                if test_match:
                    total_tests = int(test_match.group(1))
                    failures = int(test_match.group(2))
                    passed = total_tests - failures
                    
                    metrics["test_cases"] = total_tests
                    metrics["passed_tests"] = passed
                    metrics["failed_tests"] = failures
                    
                    if total_tests > 0:
                        metrics["test_success_rate"] = (passed / total_tests) * 100
                    
                    print(f"✅ Found test results: {total_tests} tests, {passed} passed, {failures} failed")
                else:
                    print("❌ Could not parse test results")
                    # 다른 패턴들 시도
                    alt_patterns = [
                        r'(\d+) test cases: (\d+) succeeded',
                        r'Tests run: (\d+), Failures: (\d+)',
                        r'SUMMARY.*(\d+) succeeded.*(\d+) failed'
                    ]
                    for pattern in alt_patterns:
                        alt_match = re.search(pattern, content, re.IGNORECASE | re.DOTALL)
                        if alt_match:
                            print(f"📝 Alternative pattern matched: {pattern}")
                            break
                
                # 실행 시간 추출 ("Took 0.86 seconds" 패턴)
                time_match = re.search(r'Took (\d+\.\d+) seconds', content)
                if time_match:
                    metrics["execution_time"] = float(time_match.group(1))
                    print(f"⏱️ Execution time: {metrics['execution_time']}s")
                    
            except Exception as e:
                print(f"❌ Error reading test log: {e}")
                pass
        else:
            print(f"❌ Test log not found: {test_log_path}")
        
        # 커버리지 추정 (테스트 파일 수 대비 소스 파일 수)
        source_files = len(list(self.src_dir.rglob("*.cpp")))
        if source_files > 0 and metrics["test_files"] > 0:
            metrics["coverage_estimate"] = min(100.0, (metrics["test_files"] / source_files) * 100)
        
        return metrics

    def collect_build_metrics(self) -> Dict[str, Any]:
        """빌드 메트릭 수집"""
        print("🔨 Collecting build metrics...")
        
        metrics = {
            "compilation_success": False,
            "ram_usage_percent": 0.0,
            "flash_usage_percent": 0.0,
            "binary_size_bytes": 0,
            "warnings_count": 0,
            "errors_count": 0
        }
        
        # 빌드 로그 파일 확인
        build_log_path = self.project_root / "logs" / "comfile" / "compile_results.txt"
        
        if build_log_path.exists():
            try:
                # 여러 인코딩 방식 시도 (UTF-16 BOM 포함)
                for encoding in ['utf-16', 'utf-8', 'latin-1', 'cp1252']:
                    try:
                        with open(build_log_path, 'r', encoding=encoding) as f:
                            content = f.read()
                        print(f"✅ Successfully decoded build log with {encoding}")
                        break
                    except UnicodeDecodeError:
                        continue
                else:
                    print("❌ Could not decode build log file")
                    return metrics
                
                print(f"📋 Reading build log: {build_log_path}")
                
                # 메모리 사용량 파싱
                ram_match = re.search(r'RAM:\s+\[.*?\]\s+(\d+\.\d+)%', content)
                if ram_match:
                    metrics["ram_usage_percent"] = float(ram_match.group(1))
                    print(f"💾 RAM Usage: {metrics['ram_usage_percent']}%")
                
                flash_match = re.search(r'Flash:\s+\[.*?\]\s+(\d+\.\d+)%', content)
                if flash_match:
                    metrics["flash_usage_percent"] = float(flash_match.group(1))
                    print(f"💽 Flash Usage: {metrics['flash_usage_percent']}%")
                
                # 컴파일 성공 여부
                if "SUCCESS" in content:
                    metrics["compilation_success"] = True
                    print("✅ Compilation: SUCCESS")
                else:
                    print("❌ Compilation: FAILED")
                
                # 경고/오류 개수 (간단한 추정)
                metrics["warnings_count"] = content.count("warning:")
                metrics["errors_count"] = content.count("error:")
                
                if metrics["warnings_count"] > 0:
                    print(f"⚠️ Warnings: {metrics['warnings_count']}")
                if metrics["errors_count"] > 0:
                    print(f"🚨 Errors: {metrics['errors_count']}")
                
            except Exception as e:
                print(f"❌ Error reading build log: {e}")
                pass
        else:
            print(f"❌ Build log not found: {build_log_path}")
        
        return metrics

    def calculate_quality_score(self) -> float:
        """종합 품질 점수 계산 (0-100)"""
        scores = []
        weights = []
        
        # 아키텍처 점수 (가중치: 30%)
        if "architecture_metrics" in self.metrics:
            scores.append(self.metrics["architecture_metrics"]["architecture_score"])
            weights.append(0.3)
        
        # 테스트 점수 (가중치: 25%)
        if "test_metrics" in self.metrics:
            test_score = self.metrics["test_metrics"]["test_success_rate"]
            scores.append(test_score)
            weights.append(0.25)
        
        # 빌드 점수 (가중치: 20%)
        if "build_metrics" in self.metrics:
            build_score = 0.0
            if self.metrics["build_metrics"]["compilation_success"]:
                build_score += 50.0
            # RAM/Flash 사용률이 낮을수록 좋음
            ram_score = max(0, 100 - self.metrics["build_metrics"]["ram_usage_percent"])
            flash_score = max(0, 100 - self.metrics["build_metrics"]["flash_usage_percent"])
            build_score += (ram_score + flash_score) / 4
            scores.append(build_score)
            weights.append(0.2)
        
        # 코드 품질 점수 (가중치: 15%)
        if "code_metrics" in self.metrics:
            code_score = 100.0
            avg_complexity = self.metrics["code_metrics"].get("avg_complexity", 0)
            if avg_complexity > 10:  # 평균 복잡도가 10을 넘으면 감점
                code_score -= min(50, (avg_complexity - 10) * 5)
            scores.append(code_score)
            weights.append(0.15)
        
        # 문서화 점수 (가중치: 10%)
        docs_score = 50.0  # 기본값
        if self.docs_dir.exists():
            doc_files = len(list(self.docs_dir.rglob("*.md")))
            docs_score = min(100.0, doc_files * 10)
        scores.append(docs_score)
        weights.append(0.1)
        
        # 가중 평균 계산
        if scores and weights:
            weighted_sum = sum(score * weight for score, weight in zip(scores, weights))
            total_weight = sum(weights)
            return weighted_sum / total_weight if total_weight > 0 else 0.0
        
        return 0.0

    def generate_report(self) -> str:
        """품질 메트릭 리포트 생성"""
        # 모든 메트릭 수집
        self.metrics["code_metrics"] = self.collect_code_metrics()
        self.metrics["architecture_metrics"] = self.collect_architecture_metrics()
        self.metrics["test_metrics"] = self.collect_test_metrics()
        self.metrics["build_metrics"] = self.collect_build_metrics()
        self.metrics["quality_score"] = self.calculate_quality_score()
        
        # 리포트 생성
        report = f"""
# DS18B20 Embedded Application - Code Quality Report
Generated: {self.metrics['timestamp']}

## 📊 Overall Quality Score: {self.metrics['quality_score']:.1f}/100

### 📈 Code Metrics
- **Source Files**: {self.metrics['code_metrics']['source_files']} (.cpp)
- **Header Files**: {self.metrics['code_metrics']['header_files']} (.h)
- **Test Files**: {self.metrics['code_metrics']['test_files']}
- **Total Lines**: {self.metrics['code_metrics']['total_lines']:,}
- **Functions**: {self.metrics['code_metrics']['functions']}
- **Classes**: {self.metrics['code_metrics']['classes']}
- **Average Complexity**: {self.metrics['code_metrics']['avg_complexity']:.1f}

### 🏗️ Architecture Metrics (Score: {self.metrics['architecture_metrics']['architecture_score']:.1f}/100)
- **Layer Separation**: {self.metrics['architecture_metrics']['layer_separation']['score']:.1f}/100
  - Domain files: {self.metrics['architecture_metrics']['layer_separation']['domain_files']}
  - Application files: {self.metrics['architecture_metrics']['layer_separation']['application_files']}
  - Infrastructure files: {self.metrics['architecture_metrics']['layer_separation']['infrastructure_files']}
- **Dependency Inversion**: {self.metrics['architecture_metrics']['dependency_inversion']['score']:.1f}/100
  - Interfaces: {self.metrics['architecture_metrics']['dependency_inversion']['interfaces_count']}
  - Implementations: {self.metrics['architecture_metrics']['dependency_inversion']['implementations_count']}
- **Interface Usage**: {self.metrics['architecture_metrics']['interface_usage']['score']:.1f}/100
  - Mock files: {self.metrics['architecture_metrics']['interface_usage']['mock_files_count']}

### 🧪 Test Metrics
- **Test Files**: {self.metrics['test_metrics']['test_files']}
- **Test Cases**: {self.metrics['test_metrics']['test_cases']}
- **Success Rate**: {self.metrics['test_metrics']['test_success_rate']:.1f}%
- **Execution Time**: {self.metrics['test_metrics']['execution_time']:.3f}s
- **Coverage Estimate**: {self.metrics['test_metrics']['coverage_estimate']:.1f}%

### 🔨 Build Metrics
- **Compilation**: {'✅ Success' if self.metrics['build_metrics']['compilation_success'] else '❌ Failed'}
- **RAM Usage**: {self.metrics['build_metrics']['ram_usage_percent']:.1f}%
- **Flash Usage**: {self.metrics['build_metrics']['flash_usage_percent']:.1f}%
- **Warnings**: {self.metrics['build_metrics']['warnings_count']}
- **Errors**: {self.metrics['build_metrics']['errors_count']}

## 📋 Recommendations

"""
        
        # 권장사항 추가
        if self.metrics['quality_score'] < 70:
            report += "- 🚨 Overall quality score is below 70. Consider improving architecture and test coverage.\n"
        
        if self.metrics['architecture_metrics']['dependency_inversion']['score'] < 80:
            report += "- 🏗️ Consider adding more interfaces to improve dependency inversion.\n"
        
        if self.metrics['test_metrics']['test_success_rate'] < 95:
            report += "- 🧪 Test success rate is below 95%. Fix failing tests.\n"
        
        if self.metrics['build_metrics']['ram_usage_percent'] > 80:
            report += "- 💾 RAM usage is high (>80%). Consider memory optimization.\n"
        
        if self.metrics['code_metrics']['avg_complexity'] > 10:
            report += "- 🔄 Average function complexity is high. Consider refactoring complex functions.\n"
        
        return report

    def save_metrics(self, output_path: str):
        """메트릭을 JSON 파일로 저장"""
        with open(output_path, 'w', encoding='utf-8') as f:
            json.dump(self.metrics, f, indent=2, ensure_ascii=False)
        print(f"📁 Metrics saved to: {output_path}")

def main():
    """메인 실행 함수"""
    # 프로젝트 루트 디렉토리 찾기
    current_dir = Path(__file__).parent
    project_root = current_dir.parent.parent
    
    print(f"🔍 Analyzing project: {project_root}")
    
    # 메트릭 수집기 초기화
    collector = CodeMetricsCollector(str(project_root))
    
    # 리포트 생성
    report = collector.generate_report()
    
    # 결과 출력
    print(report)
    
    # 결과 저장
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    
    # JSON 메트릭 저장
    json_path = project_root / "logs" / "quality" / f"metrics_{timestamp}.json"
    json_path.parent.mkdir(parents=True, exist_ok=True)
    collector.save_metrics(str(json_path))
    
    # 마크다운 리포트 저장
    md_path = project_root / "logs" / "quality" / f"quality_report_{timestamp}.md"
    with open(md_path, 'w', encoding='utf-8') as f:
        f.write(report)
    print(f"📄 Report saved to: {md_path}")
    
    print(f"\n✅ Quality analysis complete! Overall score: {collector.metrics['quality_score']:.1f}/100")

if __name__ == "__main__":
    main()
