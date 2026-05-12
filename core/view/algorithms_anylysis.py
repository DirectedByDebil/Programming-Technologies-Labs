import math
import numpy as np
from scipy import stats
import pandas as pd

def calculate_complexity(df: pd.DataFrame, algorithm: str, size_column: str = 'size', time_column: str = 'time'):
    """
    Вычисляет аппроксимированную сложность алгоритма на основе замеров для worst-case
    
    Args:
        df: DataFrame с данными
        algorithm: название алгоритма
        size_column: колонка с размером массива
        time_column: колонка с временем
    
    Returns:
        dict: коэффициенты для разных сложностей
    """
    # Берём только worst-case для данного алгоритма
    alg_data = df[(df['algorithm'] == algorithm) & (df['case'] == 'worst')]
    
    if alg_data.empty:
        print(f"Нет данных для алгоритма {algorithm}")
        return None
    
    # Сортируем по размеру
    alg_data = alg_data.sort_values(size_column)
    sizes = alg_data[size_column].values
    times = alg_data[time_column].values
    
    # Вычисляем отношения для разных сложностей
    n = len(sizes)
    if n < 2:
        print(f"Недостаточно данных для {algorithm}")
        return None
    
    results = {}
    
    # O(1) - константная
    constant_vals = times / 1.0
    results['O(1)'] = {
        'mean_error': float('inf'),
        'ratio_mean': constant_vals.mean(),
        'ratio_std': constant_vals.std()
    }
    
    # O(n) - линейная
    linear_vals = times / sizes
    results['O(n)'] = {
        'mean_error': float('inf'),
        'ratio_mean': linear_vals.mean(),
        'ratio_std': linear_vals.std()
    }
    
    # O(n log n)
    nlogn_vals = times / (sizes * np.log2(sizes))
    results['O(n log n)'] = {
        'mean_error': float('inf'),
        'ratio_mean': nlogn_vals.mean(),
        'ratio_std': nlogn_vals.std()
    }
    
    # O(n²) - квадратичная
    quadratic_vals = times / (sizes * sizes)
    results['O(n²)'] = {
        'mean_error': float('inf'),
        'ratio_mean': quadratic_vals.mean(),
        'ratio_std': quadratic_vals.std()
    }
    
    # Вычисляем среднеквадратичное отклонение как меру "плоскости"
    for complexity in results:
        vals = results[complexity]['ratio_mean'] + results[complexity]['ratio_std']
        results[complexity]['score'] = results[complexity]['ratio_std'] / (results[complexity]['ratio_mean'] + 1e-10)
    
    # Находим лучшую сложность (наименьшее отклонение)
    best_complexity = min(results.keys(), key=lambda x: results[x]['score'])
    
    # Вывод результатов
    print(f"\n{'='*60}")
    print(f"Анализ сложности алгоритма: {algorithm}")
    print(f"{'='*60}")
    print(f"{'Сложность':<12} | {'Ср. коэфф.':<12} | {'Отклонение':<12} | {'Стабильность':<12}")
    print(f"{'-'*60}")
    
    for complexity in ['O(1)', 'O(n)', 'O(n log n)', 'O(n²)']:
        coeff = results[complexity]['ratio_mean']
        std = results[complexity]['ratio_std']
        stability = '' if complexity == best_complexity else ' '
        print(f"{complexity:<12} | {coeff:<12.4f} | {std:<12.4f} | {stability:<12}")
    
    print(f"{'-'*60}")
    print(f"Наиболее вероятная сложность: {best_complexity}")
    print(f"   (наименьшее относительное отклонение: {results[best_complexity]['score']:.4f})")
    
    # Дополнительная аналитика
    if n >= 3:
        # Проверка роста времени при увеличении n в 2 раза
        mid = n // 2
        ratio_2x = times[-1] / times[0] if times[0] > 0 else 0
        size_ratio = sizes[-1] / sizes[0] if sizes[0] > 0 else 0
        
        expected_ratio = {
            'O(1)': 1,
            'O(n)': size_ratio,
            'O(n log n)': size_ratio * (math.log2(sizes[-1]) / math.log2(sizes[0]) if sizes[0] > 1 else 1),
            'O(n²)': size_ratio ** 2
        }
        
        print(f"\nПрактическая проверка:")
        print(f"   Рост размера: {sizes[0]} → {sizes[-1]} (в {size_ratio:.1f} раз)")
        print(f"   Рост времени: {times[0]:.2f} → {times[-1]:.2f} мс (в {ratio_2x:.1f} раз)")
        
        for complexity, expected in expected_ratio.items():
            dev = abs(ratio_2x - expected) / expected if expected > 0 else float('inf')
            match = "✓" if dev < 0.5 else " "
            print(f"   {complexity:<10} ожидаемый рост: {expected:.1f}x, отклонение: {dev:.1%} {match}")
    
    return results

# Альтернативный вариант — вычисление степенной зависимости (более точный)
def calculate_complexity_power_law(df: pd.DataFrame, algorithm: str):
    """
    Вычисляет степенной показатель n^k, где k определяет сложность
    """
    
    alg_data = df[(df['algorithm'] == algorithm) & (df['case'] == 'worst')].sort_values('size')
    
    if len(alg_data) < 2:
        print(f"Недостаточно данных для {algorithm}")
        return
    
    sizes = alg_data['size'].values
    times = alg_data['time'].values
    
    # Логарифмируем для линейной регрессии
    log_sizes = np.log(sizes)
    log_times = np.log(times)
    
    # Линейная регрессия: log(T) = k * log(n) + log(c)
    slope, intercept, r_value, p_value, std_err = stats.linregress(log_sizes, log_times)
    
    print(f"\n{'='*60}")
    print(f" Степенная аппроксимация для алгоритма: {algorithm}")
    print(f"{'='*60}")
    print(f"   T(n) ≈ {np.exp(intercept):.4f} * n^{slope:.4f}")
    print(f"   Коэффициент детерминации R²: {r_value**2:.4f}")
    print(f"\nИнтерпретация:")
    
    if slope < 0.5:
        print(f"   ~O(1) или O(log n) — константная или логарифмическая")
    elif slope < 1.2:
        print(f"   ~O(n) — линейная сложность")
    elif slope < 1.6:
        print(f"   ~O(n log n) — линейно-логарифмическая")
    elif slope < 2.2:
        print(f"   ~O(n²) — квадратичная")
    else:
        print(f"   ~O(n^{slope:.2f}) — сверхквадратичная")
    
    return slope, r_value**2