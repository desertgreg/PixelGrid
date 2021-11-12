
namespace BinaryToSnd
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.FileListView = new System.Windows.Forms.ListView();
            this.OutputFileTextBox = new System.Windows.Forms.TextBox();
            this.SelectFilesButton = new System.Windows.Forms.Button();
            this.SelectOutputButton = new System.Windows.Forms.Button();
            this.ProcessFiles = new System.Windows.Forms.Button();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            this.openFileDialog1.Filter = "raw files | *.raw";
            this.openFileDialog1.Multiselect = true;
            // 
            // FileListView
            // 
            this.FileListView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.FileListView.HideSelection = false;
            this.FileListView.Location = new System.Drawing.Point(9, 42);
            this.FileListView.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.FileListView.Name = "FileListView";
            this.FileListView.Size = new System.Drawing.Size(622, 283);
            this.FileListView.TabIndex = 0;
            this.FileListView.UseCompatibleStateImageBehavior = false;
            this.FileListView.View = System.Windows.Forms.View.List;
            // 
            // OutputFileTextBox
            // 
            this.OutputFileTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.OutputFileTextBox.Location = new System.Drawing.Point(9, 375);
            this.OutputFileTextBox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.OutputFileTextBox.Name = "OutputFileTextBox";
            this.OutputFileTextBox.Size = new System.Drawing.Size(622, 20);
            this.OutputFileTextBox.TabIndex = 1;
            // 
            // SelectFilesButton
            // 
            this.SelectFilesButton.Location = new System.Drawing.Point(9, 19);
            this.SelectFilesButton.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.SelectFilesButton.Name = "SelectFilesButton";
            this.SelectFilesButton.Size = new System.Drawing.Size(85, 19);
            this.SelectFilesButton.TabIndex = 2;
            this.SelectFilesButton.Text = "Select Files";
            this.SelectFilesButton.UseVisualStyleBackColor = true;
            this.SelectFilesButton.Click += new System.EventHandler(this.SelectFilesButton_Click);
            // 
            // SelectOutputButton
            // 
            this.SelectOutputButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.SelectOutputButton.Location = new System.Drawing.Point(9, 351);
            this.SelectOutputButton.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.SelectOutputButton.Name = "SelectOutputButton";
            this.SelectOutputButton.Size = new System.Drawing.Size(94, 19);
            this.SelectOutputButton.TabIndex = 3;
            this.SelectOutputButton.Text = "Select Output File";
            this.SelectOutputButton.UseVisualStyleBackColor = true;
            this.SelectOutputButton.Click += new System.EventHandler(this.SelectOutputButton_Click);
            // 
            // ProcessFiles
            // 
            this.ProcessFiles.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.ProcessFiles.Location = new System.Drawing.Point(543, 418);
            this.ProcessFiles.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.ProcessFiles.Name = "ProcessFiles";
            this.ProcessFiles.Size = new System.Drawing.Size(88, 19);
            this.ProcessFiles.TabIndex = 4;
            this.ProcessFiles.Text = "Generate";
            this.ProcessFiles.UseVisualStyleBackColor = true;
            this.ProcessFiles.Click += new System.EventHandler(this.ProcessFiles_Click);
            // 
            // saveFileDialog1
            // 
            this.saveFileDialog1.DefaultExt = "cpp";
            this.saveFileDialog1.Filter = "CPP Files | *.cpp";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(99, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(413, 13);
            this.label1.TabIndex = 5;
            this.label1.Text = "Your raw sound file names should end with (1K,2K,4K,8K) to indicate their sample " +
    "rate.";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(640, 447);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.ProcessFiles);
            this.Controls.Add(this.SelectOutputButton);
            this.Controls.Add(this.SelectFilesButton);
            this.Controls.Add(this.OutputFileTextBox);
            this.Controls.Add(this.FileListView);
            this.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.Name = "Form1";
            this.Text = "BinaryToSnd";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.ListView FileListView;
        private System.Windows.Forms.TextBox OutputFileTextBox;
        private System.Windows.Forms.Button SelectFilesButton;
        private System.Windows.Forms.Button SelectOutputButton;
        private System.Windows.Forms.Button ProcessFiles;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.Label label1;
    }
}

