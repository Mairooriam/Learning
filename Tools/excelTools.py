import pandas as pd
import sys
import os

def excel_to_csv(excel_path, csv_path=None):
    try:
        # Read Excel file
        df = pd.read_excel(excel_path, converters={col: lambda x: str(x).replace('\n', ' ') if isinstance(x, str) else x 
                              for col in pd.read_excel(excel_path, nrows=0).columns})
        # If no CSV path specified, create one from Excel path
        if csv_path is None:
            csv_path = os.path.splitext(excel_path)[0] + '.csv'
        
        # Write to CSV with UTF-8 encoding, using comma as separator
        df.to_csv(csv_path, index=False, encoding='utf-8', sep=',')
        print(f"Successfully converted {excel_path} to {csv_path}")
        return True
    except Exception as e:
        print(f"Error converting file: {str(e)}")
        return False



if __name__ == "__main__":
    # For debugging: Comment out sys.argv code and use direct paths
    excel_file = r"C:\Users\35850\Desktop\repositories\learning2\Learning\Mir\External\testdata\manual_edited.xlsx"
    csv_file = r"C:\Users\35850\Desktop\repositories\learning2\Learning\Mir\External\testdata\outputtest2222313.csv"
    excel_to_csv(excel_file, csv_file)

    # Original code (commented out during debug)
    # if len(sys.argv) < 2:
    #     print("Usage: python excelTools.py <excel_file_path> [csv_file_path]")
    #     sys.exit(1)
    # excel_file = sys.argv[1]
    # csv_file = sys.argv[2] if len(sys.argv) > 2 else None
    # excel_to_csv(excel_file, csv_file)