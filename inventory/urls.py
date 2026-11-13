

# inventory/urls.py
from django.urls import path
from . import views

urlpatterns = [
    # Keep this matching /parts/ exactly as you had it originally
    path('parts/', views.inventory_search_view, name='part_list'),
    
    path('locate/part/<int:pk>/', views.locate_part, name='locate_part'),
    path('locate/tool/<int:pk>/', views.locate_tool, name='locate_tool'),
    # ... your existing paths (like parts/, locate/ etc.) ...
    path('parts/import/', views.bulk_import_parts, name='bulk_import_parts'),
    path('parts/export/', views.export_parts_csv, name='export_parts_csv'),
]

