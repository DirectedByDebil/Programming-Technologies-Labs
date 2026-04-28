import sys
import pandas as pd
import plot_utils as pu
import algorithms_anylysis as aa

def get_plot_data(df: pd.DataFrame):
    
    algorithms = set(df['algorithm'])
    sizes = sorted(set(df['size']))

    plot_data = []
    #print(df)

    for alg in algorithms:
        all_rows = df[df['algorithm'] == alg]

        data = {
            "algorithm": alg,
            "sizes": []
        }

        for size in sizes:
            cur = all_rows[all_rows['size'] == size]
            cases=cur[['case', 'time']]
            
            row = {
                "size": size,
                "best": 0,
                "worst": 0,
                "average": 0
            }

            cases_count = len(cases)
            for i in range(cases_count):
                case_obj=cases.iloc[i]
                row[case_obj['case']] = case_obj['time']

            data["sizes"].append(row)

        plot_data.append(data)

    return plot_data, sizes, algorithms

def create_comparison_plots(plot_data: list, sizes: list, case_name: str, excluded: list = None):
    
    if excluded is None:
        excluded = []
    
    settings = pu.PlotSettings(1, 1)
    settings.fileName = f'comparison_{case_name}'
    settings.fileIndex = 1
    settings.suptitle = f'Сравнение алгоритмов сортировки ({case_name} случай)'
    settings.title = f'Сравнение алгоритмов - {case_name}'
    settings.xLabel = 'Размер массива'
    settings.yLabel = 'Время (мс)'
    
    pu.create_figure(settings.suptitle)
    
    # Собираем все данные в массивы (фильтруем excluded)
    algorithms = []
    all_times = []
    
    for data in plot_data:
        alg = data['algorithm']
        
        # Пропускаем исключённые алгоритмы
        if alg in excluded:
            continue
            
        times = []
        for size_info in data['sizes']:
            times.append(size_info[case_name])
        
        algorithms.append(alg)
        all_times.append(times)
    
    # Если все алгоритмы исключены — не рисуем
    if not algorithms:
        print(f"⚠️ Все алгоритмы исключены для {case_name}, график не создан")
        return
    
    # Один вызов create_plot со всеми данными
    settings.x = sizes
    settings.y = all_times
    settings.labels = algorithms
    
    pu.create_plot(settings)
    pu.save_figure(f'{settings.fileName}')
    print(f"✅ График сравнения для {case_name} сохранён (исключены: {excluded})")

def view_plot_data(plot_data: list):

    
    settings = pu.PlotSettings(2, 2)
    settings.fileName = 'plots'
    settings.fileIndex = 1
    settings.suptitle = 'suptitle'

    pu.create_figure('')
    
    # plot for each algorithm
    for data in plot_data:

        alg = data['algorithm']
        print(alg)
        settings.title = alg

        settings.x = []
        best=[]
        worst=[]
        average=[]

        sizes = data['sizes']
        for size in sizes:
            settings.x.append(size['size'])
            best.append(size['best'])
            worst.append(size['worst'])
            average.append(size['average'])
            #print(size)
        
        settings.y = [best, worst, average]
        settings.labels = ['best', 'worst', 'average']
        settings.xLabel = 'Размер массива'
        settings.yLabel = 'Время (мс)'
        pu.create_plot(settings)
        settings.index +=1

    pu.save_figure(f'{settings.fileName} ({settings.fileIndex})')



args = sys.argv[1:]
print(f"Arguments passed: {args}")

if len(sys.argv) == 0:
    print("Need pass csv file to create plots!")
else:
    file_name = sys.argv[1]
    df = pd.read_csv(file_name)

    #todo check if file exists

    plot_data, sizes, algorithms = get_plot_data(df)
    view_plot_data(plot_data)

    excluded=['Bubble', 'Insert']

    create_comparison_plots(plot_data, sizes, 'best', excluded=excluded)
    create_comparison_plots(plot_data, sizes, 'worst', excluded=excluded)
    create_comparison_plots(plot_data, sizes, 'average', excluded=excluded)

    for alg in algorithms:
        #aa.calculate_complexity(df, alg)
        aa.calculate_complexity_power_law(df, alg)
    
