import argparse
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os


def parse_args():

    parser = argparse.ArgumentParser(description='Построение графиков для лабораторной работы 3')
    parser.add_argument('-tf', '--target_func', 
                        type=str, 
                        required=True,
                        help='Путь к CSV файлу с целевой функцией')
    parser.add_argument('-sr', '--simulation_res', 
                        type=str, 
                        required=True,
                        help='Путь к CSV файлу с результатами симуляции')
    
    parser.add_argument('-pt', '--plot_title', 
                        type=str, 
                        required=True,
                        help='Название графика')

    parser.add_argument('-sdir', '--save_directory', 
                        type=str, 
                        required=True,
                        help='Директория для сохранения результатов')

    args = parser.parse_args()

    return args.target_func, args.simulation_res, args.plot_title, args.save_directory


def get_plot_data(simulation_res: pd.DataFrame):

    simulation_dots = []
    simulation_dots.append({
        "x": simulation_res['x'][0],
        "y": simulation_res['y'][0],
        "label": simulation_res['label'][0]
    })

    simulation_dots.append({
        "x": simulation_res['x'][1],
        "y": simulation_res['y'][1],
        "label": simulation_res['label'][1]
    })

    return simulation_dots

def plot_contour(target_func: pd.DataFrame, dots: list, plot_title: str, save_directory: str = "./laba3_results"):

    # Преобразуем в матрицу
    x_vals = np.sort(target_func['x'].unique())
    y_vals = np.sort(target_func['y'].unique())
    X, Y = np.meshgrid(x_vals, y_vals)
    Z = target_func.pivot(index='y', columns='x', values='z').values
    
    plt.figure(figsize=(12, 10))
    
    # Контуры
    contour = plt.contour(X, Y, Z, levels=30, cmap='viridis', alpha=0.7)
    plt.colorbar(contour, label='f(x, y)')
    
    # Точки минимумов
    for dot in dots:
        plt.plot(dot['x'], dot['y'], 'o', markersize=12, 
                label=f"{dot['label']} ({dot['x']:.3f}, {dot['y']:.3f})")
    
    plt.xlabel('x')
    plt.ylabel('y')
    plt.title(f'Контурный график {plot_title}')
    plt.legend()
    plt.grid(True, alpha=0.3)
    os.makedirs(f"{save_directory}/{plot_title}", exist_ok=True)
    plt.savefig(f'{save_directory}/{plot_title}/{plot_title}_contour.png', dpi=300)
    print("✅ Контурный график сохранён")

def plot_scatter(target_func: pd.DataFrame, dots: list, plot_title: str, save_directory: str = "./laba3_results"):
    plt.figure(figsize=(12, 10))
    
    # Цветовая карта по значениям z
    scatter = plt.scatter(target_func['x'], target_func['y'], 
                        c=target_func['z'], cmap='viridis', 
                        s=10, alpha=0.7)
    plt.colorbar(scatter, label='f(x, y)')
    
    # Точки минимумов
    for dot in dots:
        plt.plot(dot['x'], dot['y'], 'o', markersize=12, 
                label=f"{dot['label']} ({dot['x']:.3f}, {dot['y']:.3f})")
    
    plt.xlabel('x')
    plt.ylabel('y')
    plt.title(f'График целевой функции {plot_title}')
    plt.legend()
    plt.grid(True, alpha=0.3)
    os.makedirs(f"{save_directory}/{plot_title}", exist_ok=True)
    plt.savefig(f'{save_directory}/{plot_title}/{plot_title}_scatter.png', dpi=300)
    print("✅ Scatter график сохранён")

def plot_3d(target_func: pd.DataFrame, dots: list, plot_title: str, save_directory: str = "./laba3_results"):
    fig = plt.figure(figsize=(14, 10))
    ax = fig.add_subplot(111, projection='3d')
    
    x_vals = np.sort(target_func['x'].unique())
    y_vals = np.sort(target_func['y'].unique())
    X, Y = np.meshgrid(x_vals, y_vals)
    Z = target_func.pivot(index='y', columns='x', values='z').values
    
    # Поверхность
    surf = ax.plot_surface(X, Y, Z, cmap='viridis', alpha=0.8, linewidth=0, antialiased=True)
    fig.colorbar(surf, ax=ax, shrink=0.5, aspect=5, label='f(x, y)')
    
    # Точки минимумов
    for dot in dots:
        # Находим ближайшую точку в grid
        x_idx = np.argmin(np.abs(x_vals - dot['x']))
        y_idx = np.argmin(np.abs(y_vals - dot['y']))
        
        z_val = Z[y_idx, x_idx]  # Z[индекс_y, индекс_x]
        
        ax.scatter(x_vals[x_idx], y_vals[y_idx], z_val, s=100, 
                label=f"{dot['label']} ({dot['x']:.3f}, {dot['y']:.3f}) [≈{z_val:.3f}]")
    
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('f(x, y)')
    ax.set_title(f'3D график {plot_title}')
    
    if dots:
        ax.legend()
    
    os.makedirs(f"{save_directory}/{plot_title}", exist_ok=True)

    plt.savefig(f'{save_directory}/{plot_title}/{plot_title}_3d.png', dpi=300, bbox_inches='tight')

    ax.view_init(elev=90, azim=-90)
    plt.savefig(f'{save_directory}/{plot_title}/{plot_title}_3d_top.png')

    # Вид сбоку
    ax.view_init(elev=0, azim=-90)
    plt.savefig(f'{save_directory}/{plot_title}/{plot_title}_3d_side.png')

    # Вид спереди
    ax.view_init(elev=0, azim=0)
    plt.savefig(f'{save_directory}/{plot_title}/{plot_title}_3d_front.png')

    plt.close()
    print("✅ 3D графики сохранены")

target_func_filename, simulation_res_filename, plot_title, save_directory = parse_args()

target_func = pd.read_csv(target_func_filename)
simulation_res = pd.read_csv(simulation_res_filename)

dots = get_plot_data(simulation_res)
print(dots)

plot_contour(target_func, dots, plot_title=plot_title)
plot_scatter(target_func, dots, plot_title=plot_title)
plot_3d(target_func, dots, plot_title=plot_title)