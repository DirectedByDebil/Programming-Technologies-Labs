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

def create_comparison_plots(plot_data: list, sizes: list, case_name: str, dir: str, excluded: list = None):
    
    if excluded is None:
        excluded = []
    
    settings = pu.PlotSettings(1, 1)
    settings.fileName = f'{dir}/comparison_{case_name}'
    settings.fileIndex = 1
    settings.suptitle = f'Сравнение алгоритмов сортировки ({case_name} случай)'
    settings.title = f'Сравнение алгоритмов - {case_name}'
    settings.xLabel = 'Размер массива'
    settings.yLabel = 'Время (мс)'
    
    pu.create_figure(settings.suptitle)
    
    algorithms = []
    all_times = []
    
    for data in plot_data:
        alg = data['algorithm']
        
        if alg in excluded:
            continue
            
        times = []
        for size_info in data['sizes']:
            times.append(size_info[case_name])
        
        algorithms.append(alg)
        all_times.append(times)
    
    if not algorithms:
        print(f"Все алгоритмы исключены для {case_name}, график не создан")
        return
    
    settings.x = sizes
    settings.y = all_times
    settings.labels = algorithms
    
    pu.create_plot(settings)
    pu.save_figure(f'{settings.fileName}')
    print(f"График сравнения для {case_name} сохранён (исключены: {excluded})")

def view_plot_data(plot_data: list, dir: str):

    
    settings = pu.PlotSettings(2, 2)
    settings.fileName = f'{dir}/plots'
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


def create_radix_comparison_plots(plot_data: list, sizes: list, dir: str):
    
    # Собираем radix алгоритмы
    radix_msd = []
    radix_lsd = []
    
    for data in plot_data:
        alg = data['algorithm']
        if 'RadixMSD' in alg:
            radix_msd.append(data)
        elif 'RadixLSD' in alg:
            radix_lsd.append(data)
    
    # Сортируем по количеству битов (1, 2, 4, 8)
    radix_msd.sort(key=lambda x: int(x['algorithm'].split('_')[1]) if '_' in x['algorithm'] else 0)
    radix_lsd.sort(key=lambda x: int(x['algorithm'].split('_')[1]) if '_' in x['algorithm'] else 0)
    
    # Для каждого типа radix создаём 3 графика (best, worst, average)
    for case_name in ['best', 'worst', 'average']:
        
        # График для RadixMSD
        if radix_msd:
            settings = pu.PlotSettings(1, 1)
            settings.fileName = f'{dir}/radix_msd_{case_name}'
            settings.fileIndex = 1
            settings.suptitle = f'Radix MSD — сравнение разрядности ({case_name} случай)'
            settings.title = f'Radix MSD ({case_name})'
            settings.xLabel = 'Размер массива'
            settings.yLabel = 'Время (мс)'
            
            pu.create_figure(settings.suptitle)
            
            algorithms = []
            all_times = []
            
            for data in radix_msd:
                alg = data['algorithm']
                bits = alg.split('_')[1] if '_' in alg else 'default'
                
                times = []
                for size_info in data['sizes']:
                    times.append(size_info[case_name])
                
                algorithms.append(f'{bits} bits')
                all_times.append(times)
            
            settings.x = sizes
            settings.y = all_times
            settings.labels = algorithms
            
            pu.create_plot(settings)
            pu.save_figure(f'{settings.fileName}')
            print(f"График Radix MSD ({case_name}) сохранён")
        
        # График для RadixLSD
        if radix_lsd:
            settings = pu.PlotSettings(1, 1)
            settings.fileName = f'{dir}/radix_lsd_{case_name}'
            settings.fileIndex = 1
            settings.suptitle = f'Radix LSD — сравнение разрядности ({case_name} случай)'
            settings.title = f'Radix LSD ({case_name})'
            settings.xLabel = 'Размер массива'
            settings.yLabel = 'Время (мс)'
            
            pu.create_figure(settings.suptitle)
            
            algorithms = []
            all_times = []
            
            for data in radix_lsd:
                alg = data['algorithm']
                bits = alg.split('_')[1] if '_' in alg else 'default'
                
                times = []
                for size_info in data['sizes']:
                    times.append(size_info[case_name])
                
                algorithms.append(f'{bits} bits')
                all_times.append(times)
            
            settings.x = sizes
            settings.y = all_times
            settings.labels = algorithms
            
            pu.create_plot(settings)
            pu.save_figure(f'{settings.fileName}')
            print(f"График Radix LSD ({case_name}) сохранён")



args = sys.argv[1:]
print(f"Arguments passed: {args}")

if len(sys.argv) == 0:
    print("Need pass csv file to create plots!")
else:
    dir = sys.argv[1]
    file_name = sys.argv[2]

    dir = dir.replace('\\', '/')
    file_name = file_name.replace('\\', '/')

    path = f'{dir}/{file_name}'
    print(path)
    df = pd.read_csv(path)
    #todo check if file exists

    plot_data, sizes, algorithms = get_plot_data(df)
    view_plot_data(plot_data, dir)

    excluded=['Bubble', 'Insert']

    create_comparison_plots(plot_data, sizes, 'best', dir, excluded=excluded)
    create_comparison_plots(plot_data, sizes, 'worst', dir, excluded=excluded)
    create_comparison_plots(plot_data, sizes, 'average', dir, excluded=excluded)

    create_radix_comparison_plots(plot_data, sizes, dir)

    #for alg in algorithms:
        #aa.calculate_complexity(df, alg)
        #aa.calculate_complexity_power_law(df, alg)
    
